# ============================================================
# стадия 1
# ============================================================
FROM ubuntu:22.04 AS builder

ENV DEBIAN_FRONTEND=noninteractive
ENV TZ=Europe/Moscow

RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone

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

COPY *.cpp ./
COPY *.h   ./
COPY *.pro ./

RUN qmake echoServer.pro CONFIG+=release CONFIG-=debug \
    && make -j$(nproc)

# ============================================================
# Stage 2: Runtime
# ============================================================
FROM ubuntu:22.04 AS runtime

ENV DEBIAN_FRONTEND=noninteractive
ENV TZ=Europe/Moscow

RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone

RUN apt-get update && apt-get install -y \
    libqt5core5a \
    libqt5network5 \
    libqt5sql5 \
    libqt5sql5-sqlite \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

COPY --from=builder /build/echoServer ./echoServer
RUN mkdir -p /app/data

ENV DB_PATH=/app/data/SQLite.db

EXPOSE 33333

ENTRYPOINT ["./echoServer"]