FROM debian:bullseye-slim

RUN apt-get update && apt-get install -y \
    g++ cmake make libtins-dev libpcap-dev iputils-ping curl net-tools \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /src

CMD ["/bin/bash"]