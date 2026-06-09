#!/bin/bash

# Вычисление даты православной Пасхи методом Гаусса
# Используется алгоритм с учётом перехода на григорианский календарь (сдвиг +13 дней)

calculate_offset() # расчёт сдвига между юлианским и григорианским календарём
{
    local target_year=$1 # год для расчёта разницы
    # Формула: столетие минус високосные века минус базовая поправка
    local shift=$(( (target_year / 100) - (target_year / 400) - 2 )); echo "$shift"
}

if [ -n "$1" ]; then INPUT_YEAR=$1 # если есть аргумент командной строки — используем его
else read -p "Укажите год для определения даты Пасхи: " INPUT_YEAR; fi # запрос года у пользователя

# Математические вычисления по методу Гаусса
remainder_19=$(( INPUT_YEAR % 19 )) # остаток от деления на 19 (цикл Луны)
remainder_4=$(( INPUT_YEAR % 4 ))   # остаток от деления на 4 (високосный год)
remainder_7=$(( INPUT_YEAR % 7 ))   # остаток от деления на 7 (день недели)
lunar_term=$(( (19 * remainder_19 + 15) % 30 )) # лунная коррекция
solar_term=$(( (2 * remainder_4 + 4 * remainder_7 + 6 * lunar_term + 6) % 7 )) # солнечная коррекция
total_offset=$(( lunar_term + solar_term )) # суммарный сдвиг

# Расчёт даты по юлианскому календарю
if [ "$total_offset" -le 9 ] # если сумма не превышает 9 — Пасха в марте
then PASCHA_DAY_OLD=$(( 22 + total_offset )); PASCHA_MONTH_OLD=3
else PASCHA_DAY_OLD=$(( total_offset - 9 )); PASCHA_MONTH_OLD=4; fi # иначе — в апреле

# Конвертация в григорианский календарь
CALENDAR_SHIFT=$(calculate_offset "$INPUT_YEAR") # получаем разницу между календарями
PASCHA_DAY_NEW=$(( PASCHA_DAY_OLD + CALENDAR_SHIFT )) # добавляем сдвиг к юлианской дате
PASCHA_MONTH_NEW=$PASCHA_MONTH_OLD # месяц пока тот же

# Обработка перехода через конец месяца
if [ "$PASCHA_MONTH_NEW" -eq 3 ] && [ "$PASCHA_DAY_NEW" -gt 31 ] # март имеет 31 день
then PASCHA_DAY_NEW=$(( PASCHA_DAY_NEW - 31 )); PASCHA_MONTH_NEW=4 # переходим в апрель
elif [ "$PASCHA_MONTH_NEW" -eq 4 ] && [ "$PASCHA_DAY_NEW" -gt 30 ] # апрель имеет 30 дней
then PASCHA_DAY_NEW=$(( PASCHA_DAY_NEW - 30 )); PASCHA_MONTH_NEW=5; fi # переходим в май

# Формирование названия месяца для вывода
case $PASCHA_MONTH_NEW in
    3) MONTH_LABEL="марта" ;;
    4) MONTH_LABEL="апреля" ;;
    5) MONTH_LABEL="мая" ;;
esac

echo -e "\nГотово!" # вывод с поддержкой управляющих последовательностей
echo "В $INPUT_YEAR году православная Пасха отмечается $PASCHA_DAY_NEW $MONTH_LABEL (новый стиль)."
echo -e "Календарь на указанный месяц (дата Пасхи выделена цветом):\n"

# Получение кодов управления цветом терминала
RED_COLOR=$(tput setaf 1 2>/dev/null || printf '\033[31m') # красный текст
COLOR_DEFAULT=$(tput sgr0 2>/dev/null || printf '\033[0m') # возврат к обычному цвету

# Отображение календаря с подсветкой нужной даты
# cal генерирует календарь, sed находит целевое число и оборачивает его в ANSI-коды цвета
cal "$PASCHA_MONTH_NEW" "$INPUT_YEAR" | sed "2,\$s/\b$PASCHA_DAY_NEW\b/${RED_COLOR}${PASCHA_DAY_NEW}${COLOR_DEFAULT}/"
