/* File:
 *   esp32-heltec-epd/draw/canvas.c
 * Description:
 *   Bit packing data structures for pixel operations on canvas of variable
 *   width X and height Y with variable "depth" pixels (bit count).
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


#include <stdlib.h>
#include <stdint.h>

// TODO: test this module

// structure containing canvas metadata, screen buffer data
typedef struct epd_canvas {
    uint16_t width;
    uint16_t height;
    uint8_t pixel_depth; // number of bits per pixel

    // square canvas region, data structure useful for fragmented data transfer over SPI
    uint8_t region_size;
    uint8_t x_regions;
    uint8_t y_regions;
    uint8_t **buffer;
} epd_canvas;

// initialize canvas
epd_canvas *epd_canvas_create( uint32_t width, uint32_t height, uint8_t pixel_depth, uint8_t region_size ) {
    epd_canvas *canvas = malloc( sizeof( epd_canvas ) );

    canvas->width = width;
    canvas->height = height;
    canvas->pixel_depth = pixel_depth;
    canvas->region_size = region_size;

    // initiate memory for canvas regions
    canvas->x_regions = ( width + region_size - 1 ) / region_size; // +region_size-1 to round up int division
    canvas->y_regions = ( height + region_size - 1 ) / region_size;
    canvas->buffer = ( uint8_t** ) malloc( sizeof( uint8_t* ) * x_regions * y_regions );
    for ( uint8_t region_x = 0; x < canvas->x_regions; x++ ) {
        for ( uint8_t region_y = 0; y < canvas->y_regions; y++ ) {
            uint16_t region_num = region_x * canvas->x_regions + region_y;

            // region dimensions
            uint8_t region_width = region_size;
            uint8_t region_height = region_size;
            if ( region_x == canvas->x_regions - 1 ) region_width = width % region_size; // edge of canvas
            if ( region_y == canvas->y_regions - 1 ) region_height = height % region_size;

            // region data size
            uint32_t bit_count = pixel_depth * region_width * region_height;
            uint32_t byte_count = ( bit_count + 7 ) / 8; // +7 to round up int division

            // allocate region

            // design note: why not separate the channels, e.g. have
            // n=pixel_depth channels/buffers for color operations? while this
            // would be very easy to do, and would make the pixel set/get
            // operations much easier, it would only really be viable for
            // pixel_depth < 3. new ePaper displays have increasingly high
            // color resolution and would require greater pixel depths. in
            // theory, packing data like this reduces RAM read/write
            // operations, speeding up the module. TODO however is a "batch get
            // channel" function, which returns a region's pixel data in the
            // Nth channel.
            canvas->buffer[ region_num ] = ( uint8_t* ) calloc( byte_count );
        }
    }

    return canvas;
}

// overwrite pixel at given physical location with value
void epd_canvas_pixel_set( epd_canvas *canvas, uint32_t pixel_x, uint32_t pixel_y, uint32_t value ) {
    if ( pixel_x >= canvas->width ) return;
    if ( pixel_y >= canvas->height ) return;

    // which region?
    uint8_t region_x = pixel_x / canvas->region_size; // truncated int division
    uint8_t region_y = pixel_y / canvas->region_size;
    uint8_t region_num = region_x * canvas->x_regions + region_y;
    uint8_t *region = canvas->buffer[ region_num ];

    // where is pixel within region?
    uint32_t region_pixel_x = pixel_x - ( region_x * canvas->region_size );
    uint32_t region_pixel_y = pixel_y - ( region_y * canvas->region_size );

    // where in region buffer is this pixel?
    uint32_t region_pixel_start_bit = ( ( region_pixel_x * canvas->region_size ) + region_pixel_y ) * canvas->pixel_depth;

    // <overwrite bits>
    uint32_t current_byte = region_pixel_start_bit / 8; // byte in region
    uint32_t current_bit = region_pixel_start_bit % 8;  // bit in byte
    if ( canvas->pixel_depth == 1 ) {
        // single bit operation, easy
        uint8_t buffer_mask = 1 << current_bit;
        region[ current_byte ] ^= region[ current_byte ] & buffer_mask; // erase original bit
        reigon[ current_byte ] |= (value << current_bit); // replace with new
    } else {
        // we have to do this weird bit wrangling because the pixel data
        // usually won't start exactly at the beginning of the byte, and may be
        // split across bytes for cases where pixel_depth > 1.
        //
        // even though the component currently only supports a display with
        // pixel_depth=1, this implementation offers future-proof support for
        // displays that require pixel_depth > 1. it is also preferable,
        // performance-wise on embedded systems, to write entire chunks of
        // bytes instead of doing single bit operations for the entire
        // pixel_depth like when pixel_depth=1.

        uint8_t relative_bit = 0; // [0, canvas->pixel_depth]
        while ( relative_bit < canvas->pixel_depth ) { // this should loop ((pixel_depth - 1) / 8) + 1 times
            // where is this data slot within current buffer byte?
            uint8_t buffer_mask = (uint8_t)(((1 << (canvas->pixel_depth - relative_bit)) - 1) << current_bit);

            // delete original buffer data in slot
            region[ current_byte ] ^= region[ current_byte ] & buffer_mask;

            // fetch and position new data from inputted value
            uint8_t new_data = (uint8_t)((( value >> relative_bit ) & ((1 << (canvas->pixel_depth - relative_bit)) - 1)) << current_bit);

            // write to data slot in buffer
            region[ current_byte ] |= new_data;

            // step forward:
            // future iterations will always be at the beginning of a new byte
            // (if this iteration's write ends mid-byte, pixel_depth is reached
            // and there are no more iterations left).
            // - relative_bit increases either from:
            //     mid-byte, so += (8 - current_bit), or
            //     from beginning of byte (current_bit=0), so += 8.
            //   iteration ends when relative_bit reaches pixel_depth, meaning
            //   all data has been written
            // - current_bit cannot be mid-byte, like in the first iteration,
            //   only 0
            // - current_byte can only ever step forward by 1
            relative_bit += 8 - current_bit;
            current_bit = 0;
            current_byte++;
        }
    }
    // </overwrite bits>
}

// fetch pixel value at given physical location
uint32_t epd_canvas_pixel_get( epd_canvas *canvas, uint32_t pixel_x, uint32_t pixel_y ) {
    if ( pixel_x >= canvas->width ) return;
    if ( pixel_y >= canvas->height ) return;

    // which region?
    uint8_t region_x = pixel_x / canvas->region_size; // truncated int division
    uint8_t region_y = pixel_y / canvas->region_size;
    uint8_t region_num = region_x * canvas->x_regions + region_y;
    uint8_t *region = canvas->buffer[ region_num ];

    // where is pixel within region?
    uint32_t region_pixel_x = pixel_x - ( region_x * canvas->region_size );
    uint32_t region_pixel_y = pixel_y - ( region_y * canvas->region_size );

    // where in region buffer is this pixel?
    uint32_t region_pixel_start_bit = ( ( region_pixel_x * canvas->region_size ) + region_pixel_y ) * canvas->pixel_depth;

    uint32_t current_byte = region_pixel_start_bit / 8; // byte in region
    uint32_t current_bit = region_pixel_start_bit % 8;  // bit in byte
    uint32_t accumulator = 0;

    if ( canvas->pixel_depth == 1 ) {
        accumulator = (region[ current_byte ] >> current_bit) & 1;
    } else {
        // same logic as pixel set function
        uint8_t relative_bit = 0;
        while ( relative_bit < canvas->pixel_depth ) {
            // where is data within byte?
            uint8_t buffer_mask = (uint8_t)(((1 << (canvas->pixel_depth - relative_bit)) - 1) << current_bit);

            // fetch data and accumulate
            accumulator = (accumulator << (canvas->pixel_depth - relative_bit)) | (( region[ current_byte ] & buffer_mask ) >> current_bit);

            // iterate
            relative_bit += ( 8 - current_bit );
            current_bit = 0;
            current_byte++;
        }
    }

    return accumulator;
}
