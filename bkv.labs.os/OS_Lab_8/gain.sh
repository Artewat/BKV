#!/bin/bash

# Настройки скрипта
DATA_DIR="accounts"
HOSTS_COUNT=14 # Укажите точное количество компьютеров в классе

echo "=== Шаг 1: Подготовка окружения ==="
# Сами создаем директорию, если её нет
mkdir -p "$DATA_DIR"
# Очищаем старые файлы, чтобы данные не перемешались
rm -f "$DATA_DIR"/pass_* echo "Директория '$DATA_DIR' готова."
echo "----------------------------------------"

echo "=== Шаг 2: Сбор и фильтрация данных по SSH ==="
# Цикл скачивания и фильтрации "на лету"
for ((i=1; i<=HOSTS_COUNT; i++)); do
    HOST="s$i"
    TARGET_FILE="$DATA_DIR/pass_$HOST"

    echo "Подключение к $HOST..."

    # Подключаемся, берем /etc/passwd, фильтруем UID от 1000 до 65000
    # и сохраняем результат локально
    ssh -o ConnectTimeout=5 "$HOST" "cat /etc/passwd" 2>/dev/null | \
    awk -F: '$3 >= 1000 && $3 < 65000' > "$TARGET_FILE"

    # Проверяем, успешно ли скачался файл и не пустой ли он
    if [ ! -s "$TARGET_FILE" ]; then
        echo "  [Предупреждение] Не удалось получить данные с $HOST или хост пуст."
        rm -f "$TARGET_FILE" # Удаляем пустой файл, чтобы не портить статистику
    fi
done
echo "Сбор данных завершен."
echo "----------------------------------------"

echo "=== Анализ активности пользователей на хостах ==="

# Проверяем, существует ли директория с данными предыдущей работы
if [ ! -d "$DATA_DIR" ] || [ -z "$(ls -A "$DATA_DIR"/pass_* 2>/dev/null)" ]; then
    echo "Ошибка: Не найдены результаты предыдущей работы в директории '$DATA_DIR'."
    echo "Пожалуйста, запустите сначала первый скрипт сбора данных."
    exit 1
fi

# Шаг 1: Получаем список всех уникальных пользователей (UID 1000-65000)
all_users=$(awk -F: '{print $1}' "$DATA_DIR"/pass_* 2>/dev/null | sort -u)

if [ -z "$all_users" ]; then
    echo "Ошибка: Список пользователей пуст."
    exit 1
fi

echo "Найдено пользователей для проверки: $(echo "$all_users" | wc -w)"
echo "--------------------------------------------------"

# Шаг 2: Обход каждого пользователя и проверка хостов
for user in $all_users; do
    inactive_hosts=""

    # Ищем, на каких хостах этот пользователь вообще существует
    for file in "$DATA_DIR"/pass_*; do
        HOST=$(basename "$file" | sed 's/pass_//')

        # Если пользователь есть в файле passwd этого хоста, проверяем его домашний каталог
        if grep -q "^${user}:" "$file"; then

            # Достаем путь к домашнему каталогу пользователя из сохраненного файла passwd
            user_home=$(grep "^${user}:" "$file" | cut -d: -f6)

            # Команда для выполнения на удаленном хосте:
            # 1. Проверяем, существует ли каталог.
            # 2. Ищем файлы, измененные ПОСЛЕ создания каталога.
            #    Так как точный btime может быть недоступен, мы проверяем,
            #    есть ли файлы внутри каталога, которые модифицировались позже, чем сам каталог.
            #    Команда 'find "дир" -newermm "дир"' находит файлы, измененные позже самой дир.

            remote_cmd="if [ -d '$user_home' ]; then
                            if [ -z \"\$(find '$user_home' -mindepth 1 -newermm '$user_home' 2>/dev/null)\" ]; then
                                echo 'INACTIVE';
                            fi
                        fi"

            # Выполняем проверку на удаленном хосте
            result=$(ssh -o ConnectTimeout=3 -o StrictHostKeyChecking=no "$HOST" "$remote_cmd" 2>/dev/null)

            if [ "$result" = "INACTIVE" ]; then
                if [ -z "$inactive_hosts" ]; then
                    inactive_hosts="$HOST"
                else
                    inactive_hosts="$inactive_hosts, $HOST"
                fi
            fi
        fi
    done

    # Выводим результат, если найдены хосты без активности
    if [ ! -z "$inactive_hosts" ]; then
        echo "Пользователь: $user"
        echo "  --> Ни разу не заходил (нет изменений в $user_home) на хостах: [ $inactive_hosts ]"
        echo "--------------------------------------------------"
    fi
done

echo "Анализ завершен."
