/* File:
 *   draw/canvas.c
 * Description:
 *   Bit packing data structures for pixel operations on canvas of variable
 *   width X and height Y with variable "depth" pixels (bit count).
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


#include <stdlib.h>
#include <stdint.h>
#include "epd.h"

// todo js test this module

// initialize canvas
epd_canvas *epd_canvas_create(uint16_t width, uint16_t height, uint8_t pixel_depth) {
    epd_canvas *canvas = malloc(sizeof(epd_canvas));

    canvas->width = width;
    canvas->height = height;
    canvas->pixel_depth = pixel_depth;

    // initiate memory for canvas
    uint32_t channel_size_bytes = (width * height + 7U) >> 3U; // +7 to round up int division

    canvas->buffer = (uint8_t**)malloc(sizeof(uint8_t*) * pixel_depth);

    for (uint8_t channel = 0; channel < pixel_depth; channel++) {
        canvas->buffer[channel] = calloc(channel_size_bytes, 1);
    }

    return canvas;
}

void epd_canvas_free(epd_canvas *canvas) {
    for (uint8_t channel = 0; channel < canvas->pixel_depth; channel++) {
        free(canvas->buffer[channel]);
    }
    free(canvas->buffer);
    free(canvas);
}

// overwrite pixel at given physical location with value
void epd_canvas_pixel_set(epd_canvas *canvas, uint16_t pixel_x, uint16_t pixel_y, uint8_t value) {
    if (pixel_x >= canvas->width) return;
    if (pixel_y >= canvas->height) return;

    uint32_t bit_position = canvas->width * pixel_y + pixel_x;
    uint32_t byte_position = bit_position >> 3;
    uint8_t relative_bit_position = bit_position & 7;
    uint8_t shifted_bit = 1 << relative_bit_position;
    for (uint8_t channel = 0; channel < canvas->pixel_depth; channel++) {
        uint8_t *current_buffer_byte = &canvas->buffer[channel][byte_position];
        *current_buffer_byte = (*current_buffer_byte & (~shifted_bit)) | (((value & (1 << channel)) >> channel) << relative_bit_position);
    }
}

// fetch pixel value at given physical location
uint8_t epd_canvas_pixel_get(epd_canvas *canvas, uint16_t pixel_x, uint16_t pixel_y) {
    if (pixel_x >= canvas->width) return 0;
    if (pixel_y >= canvas->height) return 0;

    uint8_t accumulator = 0;
    uint32_t bit_position = canvas->width * pixel_y + pixel_x;
    uint32_t byte_position = bit_position >> 3;
    uint8_t relative_bit_position = bit_position & 7;
    uint8_t shifted_bit = 1 << relative_bit_position;
    for (uint8_t channel = 0; channel < canvas->pixel_depth; channel++) {
        accumulator |= (((canvas->buffer[channel][byte_position] & shifted_bit) >> relative_bit_position) << channel);
    }

    return accumulator;
}
