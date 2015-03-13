#!/bin/sh

while [ true ]
do
  clear
  netstat -nlp --tcp --udp | grep /server
  sleep 1
done
