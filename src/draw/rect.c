/* File:
 *   draw/area.c
 * Description:
 *   Area fill, clear operations.
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

void epd_draw_rect(epd_canvas *canvas, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint8_t color) {
    epd_draw_fast_line_horizontal(canvas, x0, y0, x1 - x0, color);
    epd_draw_fast_line_horizontal(canvas, x0, y1, x1 - x0, color);
    epd_draw_fast_line_vertical(canvas, x0, y0, y1 - y0, color);
    epd_draw_fast_line_vertical(canvas, x1, y0, y1 - y0, color);
}

void epd_draw_rect_fill(epd_canvas *canvas, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint8_t color) {
    int sx = x0 < x1 ? 1 : -1;
    int sy = y0 < y1 ? 1 : -1;
    for (uint16_t x = x0; x != x1; x += sx) {
        for (uint16_t y = y0; y != y1; y += sy) {
            epd_canvas_pixel_set(canvas, x, y, color);
        }
    }
}
