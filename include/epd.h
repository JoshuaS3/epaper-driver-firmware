/* File:
 *   epd.h
 * Description:
 *   Header with public functions from epaper-driver-firmware, a library for
 *   driving ePaper displays from embedded microcontrollers.
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

#ifndef EMBEDDED_EPAPER_DRIVER_H_
#define EMBEDDED_EPAPER_DRIVER_H_

#include <stdint.h>

// structure containing canvas metadata, screen buffer data
typedef struct epd_canvas {
    uint16_t width;
    uint16_t height;
    uint8_t pixel_depth; // number of bits per pixel
    uint8_t **buffer;
} epd_canvas;

epd_canvas *epd_canvas_create(uint16_t width, uint16_t height, uint8_t pixel_depth);
void epd_canvas_free(epd_canvas *canvas);
void epd_canvas_pixel_set(epd_canvas *canvas, uint16_t pixel_x, uint16_t pixel_y, uint8_t value);
uint8_t epd_canvas_pixel_get(epd_canvas *canvas, uint16_t pixel_x, uint16_t pixel_y);

// line drawing
void epd_draw_line(epd_canvas *canvas, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint8_t color);
void epd_draw_line_delta(epd_canvas *canvas, uint16_t x0, uint16_t y0, int16_t dx, int16_t dy, uint8_t color);
void epd_draw_fast_line_horizontal(epd_canvas *canvas, uint16_t x0, uint16_t y0, int16_t dx, uint8_t color);

// ellipse drawing
void epd_draw_circle(epd_canvas *canvas, uint16_t x0, uint16_t y0, uint16_t radius, uint8_t color);

// font rendering
extern const uint8_t FONT_SANSSERIF24[];
extern const uint8_t FONT_SANSSERIF18[];
extern const uint8_t FONT_SANSSERIF14[];
extern const uint8_t FONT_SANSSERIF12[];
extern const uint8_t FONT_SANSSERIF10[];
extern const uint8_t FONT_SANSSERIF8[];

extern const uint8_t FONT_SERIF24[];
extern const uint8_t FONT_SERIF18[];
extern const uint8_t FONT_SERIF14[];
extern const uint8_t FONT_SERIF12[];
extern const uint8_t FONT_SERIF10[];
extern const uint8_t FONT_SERIF8[];

extern const uint8_t FONT_COURIER10[];
extern const uint8_t FONT_COURIER12[];
extern const uint8_t FONT_COURIER15[];

void epd_text_prints(epd_canvas *canvas, int x, int y, const char *string, const uint8_t FONT[], uint8_t color);

#endif
