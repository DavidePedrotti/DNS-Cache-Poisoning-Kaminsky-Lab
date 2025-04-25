FROM debian:bullseye-slim

RUN apt-get update && apt-get install -y dnsutils iputils-ping curl net-tools && rm -rf /var/lib/apt/lists/*

CMD ["/bin/bash"]