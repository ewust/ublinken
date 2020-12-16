#!/bin/bash


#echo 'CAPE-BONE-OCTO' | sudo tee -a /sys/devices/bone_capemgr.*/slots

#echo 'stop' | sudo tee /sys/class/remoteproc/remoteproc1/state
echo 'ledpru-fw' | sudo tee /sys/class/remoteproc/remoteproc1/firmware
echo 'start' | sudo tee /sys/class/remoteproc/remoteproc1/state

config-pin p9_31 pruout

cd /home/debian/ublinken
#LD_LIBRARY_PATH=/usr/local/lib ./ublink-ws
sudo ./ublink-ws
