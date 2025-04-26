#!/bin/bash

docker build -t attacker_image ./images/attacker/
docker build -t ns_image ./images/ns/
docker build -t server_image ./images/server/
docker build -t victim_image ./images/victim/