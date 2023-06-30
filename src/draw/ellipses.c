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

void epd_draw_circle_filled(epd_canvas *canvas, uint16_t x0, uint16_t y0, uint16_t radius, uint8_t color) {
    uint16_t t1 = radius / 16;
    int16_t x = radius;
    int16_t y = 0;
    while (x >= y) {
        epd_draw_fast_line_horizontal(canvas, x0 - x, y0 + y, 2 * x, color);
        epd_draw_fast_line_horizontal(canvas, x0 - x, y0 - y, 2 * x, color);
        epd_draw_fast_line_horizontal(canvas, x0 - y, y0 + x, 2 * y, color);
        epd_draw_fast_line_horizontal(canvas, x0 - y, y0 - x, 2 * y, color);

        y = y + 1;
        t1 = t1 + y;
        int16_t t2 = t1 - x;
        if (t2 >= 0) {
            t1 = t2;
            x = x - 1;
        }
    }
}

void epd_draw_ellipse(epd_canvas *canvas, uint16_t x_center, uint16_t y_center, uint16_t x_radius, uint16_t y_radius, uint8_t color) {
    float dx, dy, d1, d2, x, y;
    x = 0;
    y = y_radius;

    d1 = (y_radius * y_radius) - (x_radius * x_radius * y_radius) + (0.25 * x_radius * x_radius);
    dx = 2 * y_radius * y_radius * x;
    dy = 2 * x_radius * x_radius * y;

    while (dx < dy) {
        epd_canvas_pixel_set(canvas, x + x_center, y + y_center, color);
        epd_canvas_pixel_set(canvas, -x + x_center, y + y_center, color);
        epd_canvas_pixel_set(canvas, x + x_center, -y + y_center, color);
        epd_canvas_pixel_set(canvas, -x + x_center, -y + y_center, color);

        if (d1 < 0) {
            x++;
            dx = dx + (2 * y_radius * y_radius);
            d1 = d1 + dx + (y_radius * y_radius);
        } else {
            x++;
            y--;
            dx = dx + (2 * y_radius * y_radius);
            dy = dy - (2 * x_radius * x_radius);
            d1 = d1 + dx - dy + (y_radius * y_radius);
        }
    }
 
    d2 = ((y_radius * y_radius) * ((x + 0.5) * (x + 0.5))) +
         ((x_radius * x_radius) * ((y - 1) * (y - 1))) -
          (x_radius * x_radius * y_radius * y_radius);
 
    while (y >= 0) {
        epd_canvas_pixel_set(canvas, x + x_center, y + y_center, color);
        epd_canvas_pixel_set(canvas, -x + x_center, y + y_center, color);
        epd_canvas_pixel_set(canvas, x + x_center, -y + y_center, color);
        epd_canvas_pixel_set(canvas, -x + x_center, -y + y_center, color);

        if (d2 > 0) {
            y--;
            dy = dy - (2 * x_radius * x_radius);
            d2 = d2 + (x_radius * x_radius) - dy;
        } else {
            y--;
            x++;
            dx = dx + (2 * y_radius * y_radius);
            dy = dy - (2 * x_radius * x_radius);
            d2 = d2 + dx - dy + (x_radius * x_radius);
        }
    }
}

void epd_draw_ellipse_filled(epd_canvas *canvas, uint16_t x_center, uint16_t y_center, uint16_t x_radius, uint16_t y_radius, uint8_t color) {
    float dx, dy, d1, d2, x, y;
    x = 0;
    y = y_radius;

    d1 = (y_radius * y_radius) - (x_radius * x_radius * y_radius) + (0.25 * x_radius * x_radius);
    dx = 2 * y_radius * y_radius * x;
    dy = 2 * x_radius * x_radius * y;

    while (dx < dy) {
        epd_draw_fast_line_horizontal(canvas, x_center - x, y_center - y, x * 2, color);
        epd_draw_fast_line_horizontal(canvas, x_center - x, y_center + y, x * 2, color);

        if (d1 < 0) {
            x++;
            dx = dx + (2 * y_radius * y_radius);
            d1 = d1 + dx + (y_radius * y_radius);
        } else {
            x++;
            y--;
            dx = dx + (2 * y_radius * y_radius);
            dy = dy - (2 * x_radius * x_radius);
            d1 = d1 + dx - dy + (y_radius * y_radius);
        }
    }
 
    d2 = ((y_radius * y_radius) * ((x + 0.5) * (x + 0.5))) +
         ((x_radius * x_radius) * ((y - 1) * (y - 1))) -
          (x_radius * x_radius * y_radius * y_radius);
 
    while (y >= 0) {
        epd_draw_fast_line_horizontal(canvas, x_center - x, y_center - y, x * 2, color);
        epd_draw_fast_line_horizontal(canvas, x_center - x, y_center + y, x * 2, color);

        if (d2 > 0) {
            y--;
            dy = dy - (2 * x_radius * x_radius);
            d2 = d2 + (x_radius * x_radius) - dy;
        } else {
            y--;
            x++;
            dx = dx + (2 * y_radius * y_radius);
            dy = dy - (2 * x_radius * x_radius);
            d2 = d2 + dx - dy + (x_radius * x_radius);
        }
    }
}
