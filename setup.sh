#!/bin/bash

sudo apt update
sudo apt install cmake


#cd ../ && git clone https://github.com/osresearch/LEDscape.git && cd LEDscape && make
cd ../ && git clone https://github.com/ewust/ledpru && cd ledpru/pru && make && sudo make install && sudo make config && sudo make run && cd .. && make

cd ../ && git clone https://github.com/warmcat/libwebsockets.git && cd libwebsockets && mkdir build && cmake ../ && make && sudo make install

#echo 'options uio_pruss extram_pool_sz=0x800000' | sudo tee -a /etc/modprobe.d/ledscape.conf

make
sudo make install

