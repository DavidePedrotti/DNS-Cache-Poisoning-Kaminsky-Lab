#!/bin/bash

docker build -t my_dns_image ./images/dns/
docker build -t my_server_image ./images/server/