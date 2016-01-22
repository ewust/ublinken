/** \file
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include <inttypes.h>
#include <errno.h>
#include <unistd.h>
#include "ledscape.h"

#define STRIP_LEN 100


static unsigned int
h2rgb(
    unsigned int v1,
    unsigned int v2,
    unsigned int hue
)
{
    if (hue < 60)
        return v1 * 60 + (v2 - v1) * hue;
    if (hue < 180)
        return v2 * 60;
    if (hue < 240)
        return v1 * 60 + (v2 - v1) * (240 - hue);

    return v1 * 60;
}


// Convert HSL (Hue, Saturation, Lightness) to RGB (Red, Green, Blue)
//
//   hue:        0 to 359 - position on the color wheel, 0=red, 60=orange,
//                            120=yellow, 180=green, 240=blue, 300=violet
//
//   saturation: 0 to 100 - how bright or dull the color, 100=full, 0=gray
//
//   lightness:  0 to 100 - how light the color is, 100=white, 50=color, 0=black
//
static uint32_t
hsv2rgb(
    unsigned int hue,
    unsigned int saturation,
    unsigned int lightness
)
{
    unsigned int red, green, blue;
    unsigned int var1, var2;

    if (hue > 359)
        hue = hue % 360;
    if (saturation > 100)
        saturation = 100;
    if (lightness > 100)
        lightness = 100;

    // algorithm from: http://www.easyrgb.com/index.php?X=MATH&H=19#text19
    if (saturation == 0) {
        red = green = blue = lightness * 255 / 100;
    } else {
        if (lightness < 50) {
            var2 = lightness * (100 + saturation);
        } else {
            var2 = ((lightness + saturation) * 100) - (saturation * lightness);
        }
        var1 = lightness * 200 - var2;
        red = h2rgb(var1, var2, (hue < 240) ? hue + 120 : hue - 240) * 255 / 600000;
        green = h2rgb(var1, var2, hue) * 255 / 600000;
        blue = h2rgb(var1, var2, (hue >= 120) ? hue - 120 : hue + 240) * 255 / 600000;
    }
    return (red << 16) | (green << 8) | (blue << 0);
}


int
main(
	int argc,
	char ** argv
)
{
	ledscape_config_t * config = &ledscape_matrix_default;
	if (argc > 1)
	{
		config = ledscape_config(argv[1]);
		if (!config)
			return EXIT_FAILURE;
	}

	ledscape_t * const leds = ledscape_init(config, 0);
    printf("configed\n");

	time_t last_time = time(NULL);
	unsigned last_frame_num = 0;


	unsigned frame_num = 0;
	uint32_t * const p = calloc(STRIP_LEN*48,4);  // Have to actually allocate 48 height
	memset(p, 0x0, STRIP_LEN*48*4);

    for (int i=0; i<STRIP_LEN*48; i++) {
        if (i > STRIP_LEN*21 && i <STRIP_LEN*22)    // 18, 19 for GPIO0_2, (4,5) for GPIO0_23, (5,6) for GPIO0_26, (3,4) for GPIO1_15, (2,3) for GPIO1_14... (1,2) for GPIO2_1, (21,22) for GPIO0_3
            p[i] = (i % 0x30) << 8;
    }
    printf("ok\n");

    uint32_t *strip = &p[STRIP_LEN*1];    // So we can access GPIO2_1 directly [0..143]


    int start = STRIP_LEN;

    char r = 0xff;
    char g = 0xff;
    char b = 0xff;
    int dir = 1;

	while (1)
	{
    		//ledscape_frame_t * const frame = ledscape_frame(leds, frame_num);

    		//render(frame);

    		// wait for the previous frame to finish;

        for (int i=0; i<STRIP_LEN; i++) {
            strip[i] = 0;
            if (dir < 0 && i > start) {
                int n = ((i-start)*(i-start)) / 2;
                if (n == 0) n = 1;
                strip[i] = (r/n) | ((g/n)<<8) | ((b/n)<<16);
            } else if (dir > 0 && i < start) {
                int n = ((start-i)*(start-i))/2;
                if (n==0) n = 1;
                strip[i] = (r/n) | ((g/n)<<8) | ((b/n)<<16);
            }
        }
        start+= 1*dir;
        if (start < 0) {
            dir = 1;
            start = 1;
            r = rand() & 0xff;
            g = rand() & 0xff;
            b = rand() & 0xff;
        } else if (start > STRIP_LEN) {
            dir = -1;
            start = STRIP_LEN;
            r = rand() & 0xff;
            g = rand() & 0xff;
            b = rand() & 0xff;
        }

		for (int i=0; i<144; i++) {
			strip[i] = hsv2rgb((0+(frame_num/2))%360, 50, 20);
		}

        ledscape_draw(leds, p);
        //ledscape_wait(leds);
		usleep(20000);
		frame_num++;

		const uint32_t response = 0;
		time_t now = time(NULL);
		if (now != last_time)
		{
			printf("%d fps. starting %d previous %"PRIx32"\n",
				frame_num - last_frame_num, frame_num, response);
			last_frame_num = frame_num;
			last_time = now;
		}



	}

	ledscape_close(leds);
	return EXIT_SUCCESS;
}
