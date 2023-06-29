/* File:
 *   draw/text.c
 * Description:
 *   Font rendering utility.
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
#include "epd.h"

void epd_text_prints(epd_canvas *canvas, int x, int y, const char *string, const uint8_t FONT[], uint8_t color) {
    uint32_t px = x;
    uint32_t py = y;
    uint32_t first_glyph_byte = FONT[0];
    uint32_t last_glyph_byte = FONT[1];
    uint32_t fallback_glyph_byte = FONT[4];

    // step through every character in input string until null termination
    uint16_t current_char_index = 0;
    char current_char = string[current_char_index];
    while (current_char != '\0') {
        // out of range
        if (current_char < first_glyph_byte || current_char > last_glyph_byte) current_char = fallback_glyph_byte;
        
        // iterate through font data to find current_char
        uint32_t seeker = 5;
        for (uint8_t cur_glyph = first_glyph_byte; cur_glyph < current_char; cur_glyph++) {
            // first byte per char block indicates whether it is a duplicate. 0 -> original
            if (FONT[seeker] != 0) {
                seeker++; // skip over duplicate
            } else {
                seeker += FONT[seeker + 4] + 5; // use char data length to jump to next char
            }
        }

        // if the glyph for current_char is a duplicate, set current_char to the original and loop back
        if (FONT[seeker] != 0) {
            current_char = FONT[seeker];
            continue;
        }

        // found our character, can begin to draw on canvas
        uint8_t char_width = FONT[seeker + 1];
        uint8_t px2 = FONT[seeker + 2]; // relative draw pointer X
        uint8_t py2 = FONT[seeker + 3]; // relative draw pointer Y
        uint32_t data_length = FONT[seeker + 4];
        uint32_t data_start = seeker + 5;
        for (uint32_t byte = data_start; byte < data_start + data_length; byte++) {
            uint8_t data_byte = FONT[byte];
            for (uint8_t bit = 0; bit < 8; bit++) {
                uint8_t value = (data_byte >> bit) & 1;
                if (value) { epd_canvas_pixel_set(canvas, px + px2, py + py2, color); }
                px2++;
                if (px2 == char_width) {
                    px2 = 0;
                    py2++;
                }
            }
        }
        px += char_width;
        current_char_index++;
        current_char = string[current_char_index];
    }
}
