/* File:
 *   esp32-heltec-epd/test/basic_draw.c
 * Description:
 *   Very basic test on canvas structure using direct pixel setting operations.
 *
 * "esp32-heltec-epd" Copyright (c) 2023 Joshua Stockin
 * <josh@joshstock.in> [https://joshstock.in] [https://github.com/JoshuaS3]
 *
 * This software is licensed and distributed under the terms of the MIT License.
 * See the MIT License in the LICENSE file of this project's root folder.
 *
 * This comment block and its contents, including this disclaimer, MUST be
 * preserved in all copies or distributions of this software's source.
 */

#include <stdio.h>
#include <stdlib.h>
#include "../heltec_epd.h"

#define CANVAS_WIDTH 296
#define CANVAS_HEIGHT 128
#define BIT_DEPTH 1

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

void print( epd_canvas *canvas, int x, int y, const char *string, const uint8_t FONT[] ) {
    int px = x;
    int py = y;
    int first_glyph = FONT[ 0 ];
    int fallback_glyph = FONT[ 4 ];
    char c;
    int current = 0;
    char next = string[ current ];
    while ( next != '\0' ) {
        if ( next < first_glyph ) next = fallback_glyph;
        int pointer = 5;
        for ( int cur_glyph = first_glyph; cur_glyph < next; cur_glyph++ ) {
            if ( FONT[ pointer ] != 0 ) {
                pointer++;
                continue;
            }
            pointer = pointer + FONT[ pointer + 4 ] + 5;
        }
        if ( FONT[ pointer ] != 0 ) {
            next = FONT[ pointer ];
            continue;
        }
        int ch_width = FONT[ pointer + 1 ];
        int px2 = FONT[ pointer + 2 ];
        int py2 = FONT[ pointer + 3 ];
        int data_length = FONT[ pointer + 4 ];
        int data_start = pointer + 5;
        for ( int byte = 0; byte < data_length; byte++ ) {
            int data_byte = FONT[ data_start + byte ];
            for ( int bit = 0; bit < 8; bit++ ) {
                int value = (data_byte >> bit) & 1;
                int tx = px + px2;
                int ty = py + py2;
                if (value) epd_canvas_pixel_set( canvas, tx, ty, value );
                px2++;
                if ( px2 == ch_width ) {
                    px2 = 0;
                    py2++;
                }
            }
        }
        px += ch_width;
        current++;
        next = string[ current ];
    }
}

int main( void ) {
    epd_canvas *canvas = epd_canvas_create( CANVAS_WIDTH, CANVAS_HEIGHT, BIT_DEPTH );
    for ( int x = 0; x < CANVAS_WIDTH; x++ ) {
        epd_canvas_pixel_set( canvas, x, x % CANVAS_HEIGHT, x );
        epd_canvas_pixel_set( canvas, x, CANVAS_HEIGHT - (x % CANVAS_HEIGHT), x );
    }
    /*print( canvas, 5, 0, "Hello, World!", FONT_SERIF24 );
    print( canvas, 5, 28, "Hello, World!", FONT_SERIF18 );
    print( canvas, 5, 28+22, "Hello, World!", FONT_SERIF14 );
    print( canvas, 5, 28+22+18, "Hello, World!", FONT_SERIF12 );
    print( canvas, 5, 28+22+18+16, "Hello, World!", FONT_SERIF10 );
    print( canvas, 5, 28+22+18+16+14, "Hello, World!", FONT_SERIF8 );
    */
    print( canvas, 5, 0, "Hello, World!", FONT_COURIER10 );
    print( canvas, 5, 14, "Hello, World!", FONT_COURIER12 );
    print( canvas, 5, 14 + 19 , "Hello, World!", FONT_COURIER15 );
    int bufsize = ( CANVAS_WIDTH * CANVAS_HEIGHT * BIT_DEPTH + 7 ) / 8;
    for ( int b = 0; b < bufsize; b++ ) {
        putchar(canvas->buffer[ b ]);
    }
    return 0;
}
