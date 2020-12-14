# ublinken
playing with LEDscape


# Installing
Need cmake

## Install/build LEDscape
cd ../ && git clone https://github.com/osresearch/LEDscape.git && cd LEDscape && make

sudo cp dts/CAPE-BONE-OCTO-00A0.dtbo /lib/firmware


## Install build libwebsockets
cd ../ && git clone https://github.com/warmcat/libwebsockets.git && cd libwebsockets && mkdir build && cmake ../ && make


sudo cp /boot/uEnv.txt /boot/uEnv.txt.backup
sudo sed -i 's/#cape_disable=capemgr.disable_partno=BB-BONELT-HDMI,BB-BONELT-HDMIN$/cape_disable=capemgr.disable_partno=BB-BONELT-HDMI,BB-BONELT-HDMIN/g' /boot/uEnv.txt
echo 'options uio_pruss extram_pool_sz=0x800000' | sudo tee -a /etc/modprobe.d/ledscape.conf
