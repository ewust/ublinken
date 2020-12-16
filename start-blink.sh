#!/bin/bash

echo "Waiting for PRU..."
until [ -f /sys/class/remoteproc/remoteproc1/firmware ]
do
     sleep 1
done

echo "PRU active"

#echo 'stop' | sudo tee /sys/class/remoteproc/remoteproc1/state
echo 'ledpru-fw' | sudo tee /sys/class/remoteproc/remoteproc1/firmware
echo 'start' | sudo tee /sys/class/remoteproc/remoteproc1/state

config-pin p9_31 pruout

cd /home/debian/ublinken
sudo ./ublink-ws
