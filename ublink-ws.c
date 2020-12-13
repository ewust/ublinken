
#include "libwebsockets.h"
#include <unistd.h>
#include <arpa/inet.h>
#include "ledscape.h"

#define STRIP_LEN 100
#define SCALE 20    // out of 100

ledscape_t *leds;   // for LEDscape
uint32_t *p;        // DMA memory region
uint32_t *strip;    // Just an offset into p (where our strand is)
struct lws_context *ctx;


void handle_frame(uint32_t *frame)
{
    int i;
    for (i=0; i<STRIP_LEN; i++) {
        uint32_t bulb = ntohl(frame[i]);
        // brightness, red, green, blue
        /*
        uint32_t a = (bulb >> 24) & 0xff;
        uint32_t r = ((bulb >> 16) & 0xff)*a/255;
        uint32_t g = ((bulb >>  8) & 0xff)*a/255;
        uint32_t b = ((bulb >>  0) & 0xff)*a/255;
        */
        uint32_t a = (bulb >> 24) & 0xff;
        uint32_t r = (bulb >> 16) & 0xff;
        uint32_t g = (bulb >>  8) & 0xff;
        uint32_t b = (bulb >>  0) & 0xff;

        //printf("i=0: r=%d, g=%d, b=%d, a=%d\n", r, g, b, a);
        //r /= 3;
        //g /= 3;
        //b /= 3;

        r = (r*SCALE)/100;
        g = (g*SCALE)/100;
        b = (b*SCALE)/100;

        strip[STRIP_LEN - i - 1] = (b) | (r<<8) | (g<<16);
    }
    ledscape_draw(leds, p);
}


int connected;

void start_connection()
{

    while (1) {
        //struct lws *ws = lws_client_connect(ctx, "blinken.eecs.umich.edu", 80, 0, "/ws/stream", "blinken.eecs.umich.edu", "blinken", NULL, -1);
        //struct lws *ws = lws_client_connect(ctx, "insecure.blinken.org", 80, 0, "/api/0/stream", "insecure.blinken.org", "blinken", NULL, -1);
        struct lws *ws = lws_client_connect(ctx, "insecure.blinken.ericw.us", 80, 0, "/api/0/stream", "insecure.blinken.ericw.us", "blinken", NULL, -1);

    	printf("connecting\n");
        connected = 1;

    	while (connected) {
            lws_service(ctx, 1000);
    	}

	sleep(3);
    }
}

static int
callback_ws(struct lws *wsi, enum lws_callback_reasons reason,
            void *user, void *in, size_t len)
{
    switch (reason) {

    case LWS_CALLBACK_CLIENT_ESTABLISHED:
        printf("dumb: LWS_CALLBACK_CLIENT_ESTABLISHED\n");
        break;

    case LWS_CALLBACK_CLOSED:
        printf("dumb: LWS_CALLBACK_CLOSED\n");
        connected = 0;
        break;

    case LWS_CALLBACK_CLIENT_RECEIVE:
        //((char *)in)[len] = '\0';
        handle_frame((uint32_t*)in);
        break;

    /* because we are protocols[0] ... */

    case LWS_CALLBACK_CLIENT_CONNECTION_ERROR:
        printf("dumb: LWS_CALLBACK_CLIENT_CONNECTION_ERROR\n");
        connected = 0;
        break;

    default:
        break;
    }

    return 0;
}


static struct lws_protocols protocols[] = {
    {
        "dumb-increment-protocol,fake-nonexistant-protocol",
        callback_ws,
        0,
        400,
        0,
        NULL
    },
    { NULL, NULL, 0, 0, 0, NULL} /* end */
};

int main()
{

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


    struct lws_context_creation_info info;
    memset(&info, 0, sizeof(info));
    info.port = CONTEXT_PORT_NO_LISTEN;
    info.iface = NULL;
    info.gid = -1;
    info.uid = -1;
    info.protocols = protocols;

    printf("Hi\n");
    ctx = lws_create_context(&info);

    start_connection();
}
