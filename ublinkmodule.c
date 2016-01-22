
#include <Python.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "ledscape.h"
#include "pru.h"

#define STRIP_LEN 100

ledscape_t *leds;   // for LEDscape
uint32_t *p;        // DMA memory region
uint32_t *strip;    // Just an offset into p (where our strand is)


// Obnoxious they didn't put this in the header file

typedef struct
{
    // in the DDR shared with the PRU
    uintptr_t pixels_dma;

    // Length in pixels of the longest LED strip.
    unsigned num_pixels;

    // write 1 to start, 0xFF to abort. will be cleared when started
    volatile unsigned command;

    // will have a non-zero response written when done
    volatile unsigned response;
} __attribute__((__packed__)) ws281x_command_t;

struct ledscape
{
    ws281x_command_t * ws281x;
    pru_t * pru;
    unsigned width;
    unsigned height;
    unsigned frame_size;
    ledscape_config_t * config;
};



static PyObject *
ublink_show(PyObject *self, PyObject *args)
{
    const uint32_t buf[200];
    int buf_len = 800;

    if (!PyArg_ParseTuple(args, "s#", buf, &buf_len)) {
        return NULL;
    }

    printf("Woo got a %d byte string\n", buf_len);

    int i;
    for (i=0; i<STRIP_LEN; i++) {
        uint32_t bulb = ntohl(buf[i]);
        // brightness, red, green, blue
        char a = (bulb >> 24) & 0xff;
        char r = (bulb >> 16) & 0xff;
        char g = (bulb >>  8) & 0xff;
        char b = (bulb >>  0) & 0xff;

        strip[i] = (b) | (g<<8) | (r<<16);
    }

    printf("leds width: %d pru ddr %p, size %d\n", leds->width, leds->pru->ddr, leds->frame_size);
    ledscape_draw(leds, p);

    return Py_BuildValue("i", 0);
}

static PyMethodDef ublink_methods[] = {
    {"show", ublink_show, METH_VARARGS},
    {NULL, NULL}
};

/*
 * Python calls this to let us initialize our module
 */
void initublink()
{
    (void) Py_InitModule("ublink", ublink_methods);

    printf("initializing...\n");
    p = calloc(STRIP_LEN*48, 4);
    memset(p, 0x0, STRIP_LEN*48*4);
    strip = &p[STRIP_LEN*1];    // GPIO2_1

    printf("ok...\n");
    ledscape_config_t config;
    config.type = LEDSCAPE_STRIP;
    config.strip_config.leds_width = STRIP_LEN;
    config.strip_config.leds_height = 1;

    printf("hmm\n");
    leds = ledscape_init((ledscape_config_t*)&config, 0);
    printf("configured %p\n", leds);

}
