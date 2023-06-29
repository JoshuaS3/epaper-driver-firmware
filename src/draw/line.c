/* File:
 *   draw/line.c
 * Description:
 *   Functions for rasterizing lines on canvas.
 *
 * "epaper-driver-firmware" Copyright (c) 2023 Joshua Stockin
 * <josh@joshstock.in> [https://joshstock.in] [https://github.com/JoshuaS3]
 *
 * This software is licensed and distributed under the terms of the MIT License.
 * See the MIT License in the LICENSE file of this project's root folder.
 *
 * This comment block and its contents, including this disclaimer, MUST be
 * preserved in all copies or distributions of this software's source.
 */

#include <stdint.h>
#include <stdlib.h>
#include "epd.h"

// bresenham algorithm
void epd_draw_line(epd_canvas *canvas, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint8_t color) {
    int16_t dx = abs(x1 - x0);
    int16_t sx = x0 < x1 ? 1 : -1;
    int16_t dy = -abs(y1 - y0);
    int16_t sy = y0 < y1 ? 1 : -1;
    int16_t error = dx + dy;

    while (1) {
        epd_canvas_pixel_set(canvas, x0, y0, color);
        if (x0 == x1 && y0 == y1) break;
        int16_t e2 = 2 * error;
        if (e2 >= dy) {
            if (x0 == x1) break;
            error = error + dy;
            x0 = x0 + sx;
        }
        if (e2 <= dx) {
            if (y0 == y1) break;
            error = error + dx;
            y0 = y0 + sy;
        }
    }
}

void epd_draw_line_delta(epd_canvas *canvas, uint16_t x0, uint16_t y0, int16_t dx, int16_t dy, uint8_t color) {
    epd_draw_line(canvas, x0, y0, x0 + dx, y0 + dy, color);
}

void epd_draw_fast_line_horizontal(epd_canvas *canvas, uint16_t x0, uint16_t y0, int16_t dx, uint8_t color) {
    int16_t sx = dx > 0 ? 1 : -1;
    int16_t fx = x0 + dx;
    for (int16_t x = x0; x != fx; x += sx) {
        epd_canvas_pixel_set(canvas, x, y0, color);
    }
}

void epd_draw_fast_line_vertical(epd_canvas *canvas, uint16_t x0, uint16_t y0, int16_t dy, uint8_t color) {
    int16_t sy = dy > 0 ? 1 : -1;
    int16_t fy = y0 + dy;
    for (int16_t y = y0; y != fy; y += sy) {
        epd_canvas_pixel_set(canvas, x0, y, color);
    }
}
