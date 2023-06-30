#include <stdio.h>
#include <stdlib.h>
#include "epd.h"
#include "test.h"

#define CANVAS_WIDTH 296
#define CANVAS_HEIGHT 128
#define BIT_DEPTH 2

int main(void) {
    epd_canvas *canvas = epd_canvas_create(CANVAS_WIDTH, CANVAS_HEIGHT, BIT_DEPTH);

    epd_draw_line(canvas, 0, 0, 296, 128, 1);
    epd_draw_line(canvas, 0, 128, 296, 0, 2);
    epd_draw_line(canvas, 0, 30, 296, 100, 3);

    epd_draw_fast_line_horizontal(canvas, 80, 120, -50, 2);

    epd_draw_circle_filled(canvas, 200, 70, 30, 1);

    epd_text_prints(canvas, 5, 0, "Hello, World!", FONT_SANSSERIF24, 1);
    epd_text_prints(canvas, 5, 28, "Hello, World!", FONT_SANSSERIF18, 2);
    epd_text_prints(canvas, 5, 50, "Hello, World!", FONT_SANSSERIF14, 3);
    epd_text_prints(canvas, 5, 68, "Hello, World!", FONT_SANSSERIF12, 1);
    epd_text_prints(canvas, 5, 84, "Hello, World!", FONT_SANSSERIF10, 2);
    epd_text_prints(canvas, 5, 98, "Hello, World!", FONT_SANSSERIF8, 3);

    epd_draw_ellipse(canvas, 250, 80, 50, 30, 1);
    epd_draw_ellipse_filled(canvas, 150, 50, 10, 20, 2);

    epd_draw_rect_fill(canvas, 20, 20, 50, 50, 1);

    dump_canvas(canvas);

    return 0;
}
