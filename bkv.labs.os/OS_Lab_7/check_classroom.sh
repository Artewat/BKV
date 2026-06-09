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

echo "=== Шаг 3: Анализ учетных записей ==="
# Собираем список ВСЕХ уникальных пользователей из оставшихся файлов
all_users=$(awk -F: '{print $1}' "$DATA_DIR"/pass_* 2>/dev/null | sort -u)

if [ -z "$all_users" ]; then
    echo "Ошибка: Не удалось собрать данные ни с одного компьютера. Проверьте SSH-ключи."
    exit 1
fi

issues_found=0

# Проверяем каждого пользователя
for user in $all_users; do
    missing_hosts=""

    # Проверяем каждый доступный файл хоста
    for file in "$DATA_DIR"/pass_*; do
        CURRENT_HOST=$(basename "$file" | sed 's/pass_//')

        # Проверяем, есть ли пользователь в конкретном файле
        if ! grep -q "^${user}:" "$file"; then
            if [ -z "$missing_hosts" ]; then
                missing_hosts="$CURRENT_HOST"
            else
                missing_hosts="$missing_hosts, $CURRENT_HOST"
            fi
        fi
    done

    # Если найдены хосты, где пользователя нет — выводим
    if [ ! -z "$missing_hosts" ]; then
        echo "Пользователь: $user"
        echo "  --> Отсутствует на: [ $missing_hosts ]"
        echo "----------------------------------------"
        issues_found=1
    fi
done

if [ $issues_found -eq 0 ]; then
    echo "Успех! Все пользователи синхронизированы и есть на всех доступных ПК."
fi
