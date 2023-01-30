#!/bin/bash

echo 'sec,user,sys,idle'

second=0

while [ $second -lt 30 ]
do
    echo -n "$second,"
    top -l 1 -n 5 -s 0 | grep "CPU usage" | sed 's/.* usage: \(.*\) user, \(.*\) sys, \(.*\) idle.*/\1,\2,\3/' | sed 's/%//g'
    sleep 1
    second=$((second+1))
done

