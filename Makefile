


CFLAGS+=-I../LEDscape/src/ledscape/ -std=c99
LDFLAGS+=-L../LEDscape/lib/ -L../LEDscape/am335x/app_loader/lib/

all: ublink-ws

test: test.c
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS) -lledscape -lpthread -lm -lprussdrv

ublink-ws: ublink-ws.c
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS) -lledscape -lpthread -lm -lprussdrv -lwebsockets

ublink: ublinkmodule.c
	$(CC) $(CFLAGS) -shared -I/usr/include/python2.7/ -lpython2.7 -fPIC -o ublink.so $^ $(LDFLAGS) -lledscape -lpthread -lm -lprussdrvd
