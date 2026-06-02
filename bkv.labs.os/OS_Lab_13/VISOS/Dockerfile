# Используем старый Debian, где еще можно найти или собрать старые библиотеки
FROM debian:stretch

# Устанавливаем базовые инструменты сборки
RUN apt-get update && apt-get install -y \
    build-essential \
    libgnomeui-dev \
    libgnome-dev \
    libgnomecanvas2-dev \
    x11-apps \
    && rm -rf /var/lib/apt/lists/*

# Копируем исходники VisualOS внутрь контейнера
COPY ./VisualOS-1.0.5 /opt/VisualOS

WORKDIR /opt/VisualOS

# Пробуем сконфигурировать и собрать
RUN ./configure && make
