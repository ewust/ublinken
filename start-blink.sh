#!/bin/bash


echo 'CAPE-BONE-OCTO' | sudo tee -a /sys/devices/bone_capemgr.*/slots

cd /home/debian/ublinken
LD_LIBRARY_PATH=/usr/local/lib ./ublink-ws
