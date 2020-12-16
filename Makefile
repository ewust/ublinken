


CFLAGS+=-I../ledpru -std=c99
LDFLAGS+=-L../ledpru

all: ublink-ws

test: test.c
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS) -lledpru -lpthread -lm

ublink-ws: ublink-ws.c
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS) -lledpru -lpthread -lm -lwebsockets

ublink: ublinkmodule.c
	$(CC) $(CFLAGS) -shared -I/usr/include/python2.7/ -lpython2.7 -fPIC -o ublink.so $^ $(LDFLAGS) -lledscape -lpthread -lm -lprussdrvd
