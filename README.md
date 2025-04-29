# DNS-Cache-Poisoning-Kaminsky-Lab
Lab for Network Security

Command to delay the legitimate responses:
tc qdisc add dev eth0 root netem delay 1000ms;