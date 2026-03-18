# ============================================================
# Stage 1: Build
# ============================================================
FROM ubuntu:22.04 AS builder

ENV DEBIAN_FRONTEND=noninteractive
ENV TZ=Europe/Moscow

RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone

# Устанавливаем Qt5 + инструменты сборки + SQLite
RUN apt-get update && apt-get install -y \
    build-essential \
    qtbase5-dev \
    qtchooser \
    qt5-qmake \
    qtbase5-dev-tools \
    libqt5sql5-sqlite \
    libqt5network5 \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /build

# Копируем исходники
COPY *.cpp ./
COPY *.h   ./
COPY *.pro ./

# Собираем в release-режиме
RUN qmake echoServer.pro CONFIG+=release CONFIG-=debug \
    && make -j$(nproc)

# ============================================================
# Stage 2: Runtime (минимальный образ)
# ============================================================
FROM ubuntu:22.04 AS runtime

ENV DEBIAN_FRONTEND=noninteractive
ENV TZ=Europe/Moscow

RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone

# Только runtime-зависимости
RUN apt-get update && apt-get install -y \
    libqt5core5a \
    libqt5network5 \
    libqt5sql5 \
    libqt5sql5-sqlite \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

# Бинарник из стадии сборки
COPY --from=builder /build/echoServer ./echoServer

# Папка для базы данных (монтируется как volume)
RUN mkdir -p /app/data

# Путь к БД через переменную окружения (можно переопределить при запуске)
ENV DB_PATH=/app/data/SQLite.db

EXPOSE 33333

ENTRYPOINT ["./echoServer"]
