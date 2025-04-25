#!/bin/bash

docker build -t my_attacker_image ./images/attacker/
docker build -t my_dns_image ./images/dns/
docker build -t my_server_image ./images/server/
docker build -t my_victim_image ./images/victim/