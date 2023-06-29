#include "epd.h"

void dump_canvas(epd_canvas *canvas) {
    for (uint16_t y = 0; y < canvas->height; y++) {
        for (uint16_t x = 0; x < canvas->width; x++) {
            putchar(epd_canvas_pixel_get(canvas, x, y));
        }
    }
}
