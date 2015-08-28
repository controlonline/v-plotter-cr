#!/bin/sh
pause=0.001
gpio -g mode 22 out
gpio -g mode 23 out


echo round $i starts:
sleep 1

gpio -g write 22 1
for i in $(seq 200); do
  echo $i
  gpio -g write 23 1
  sleep $pause
  gpio -g write 23 0
  sleep $pause
done
sleep 2
gpio -g write 22 0
for i in $(seq 200); do
  echo $i
  gpio -g write 23 1
  sleep $pause
  gpio -g write 23 0
  sleep $pause
done

