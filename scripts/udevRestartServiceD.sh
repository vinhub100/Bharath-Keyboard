#! /bin/bash

cd "${0%/*}"

sleep 1
systemctl restart kanetKB
sleep 1
systemctl restart kanetKB

#echo `systemctl status kanetKB` >> log.txt

