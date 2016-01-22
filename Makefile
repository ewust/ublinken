


CFLAGS+=-I../LEDscape/src/ledscape/ -std=c99
LDFLAGS+=-L../LEDscape/lib/ -L../LEDscape/am335x/app_loader/lib/

test: test.c
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS) -lledscape -lpthread -lm -lprussdrv
