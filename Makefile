


CFLAGS+=-I../LEDscape/src/ledscape/ -std=c99
LDFLAGS+=-L../LEDscape/lib/ -L../LEDscape/am335x/app_loader/lib/

test: test.c
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS) -lledscape -lpthread -lm -lprussdrv

ublink: ublinkmodule.c
	$(CC) -shared -I/usr/include/python2.7/ -lpython2.7 -fPIC -o ublink.so $^
