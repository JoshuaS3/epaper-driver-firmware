/* File:
 *   draw/ellipses.c
 * Description:
 *   Functions for rasterizing circles, ellipses, arcs on canvas.
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

#include "epd.h"

void epd_draw_circle(epd_canvas *canvas, uint16_t x0, uint16_t y0, uint16_t radius, uint8_t color) {
    uint16_t t1 = radius / 16;
    int16_t x = radius;
    int16_t y = 0;
    while (x >= y) {
        epd_canvas_pixel_set(canvas, x0 + x, y0 + y, color);
        epd_canvas_pixel_set(canvas, x0 - x, y0 + y, color);
        epd_canvas_pixel_set(canvas, x0 + y, y0 + x, color);
        epd_canvas_pixel_set(canvas, x0 - y, y0 + x, color);
        epd_canvas_pixel_set(canvas, x0 + x, y0 - y, color);
        epd_canvas_pixel_set(canvas, x0 - x, y0 - y, color);
        epd_canvas_pixel_set(canvas, x0 + y, y0 - x, color);
        epd_canvas_pixel_set(canvas, x0 - y, y0 - x, color);

        y = y + 1;
        t1 = t1 + y;
        int16_t t2 = t1 - x;
        if (t2 >= 0) {
            t1 = t2;
            x = x - 1;
        }
    }
}
