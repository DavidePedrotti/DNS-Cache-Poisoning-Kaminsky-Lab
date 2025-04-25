FROM debian:bullseye-slim

RUN apt-get update && apt-get install -y \
    bind9 bind9utils dnsutils iputils-ping curl net-tools \
    && rm -rf /var/lib/apt/lists/*

CMD ["named","-g"]