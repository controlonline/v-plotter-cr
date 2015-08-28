#!/bin/sh
pause=0.001
gpio -g mode 4 out
gpio -g mode 17 out


echo round $i starts:
sleep 1

gpio -g write 17 1
for i in $(seq 200); do
  echo $i
  gpio -g write 4 1
  sleep $pause
  gpio -g write 4 0
  sleep $pause
done
sleep 2
gpio -g write 17 0
for i in $(seq 200); do
  echo $i
  gpio -g write 4 1
  sleep $pause
  gpio -g write 4 0
  sleep $pause
done

