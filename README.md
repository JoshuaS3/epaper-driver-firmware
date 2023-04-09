# ESP-IDF driver for 2.90" BW Heltec E-Ink display

Not functional yet, do not use.

Eventually this will be a working ESP-IDF driving component for Heltec 2.90" BW
ePaper displays (EPDs). I'm planning on future-proofing this as much as
possible, so it could become a general use EPD component for potentially any
microcontroller and EPD. This project shall be split into distinct parts, by
folder:

- `draw/`: very lightweight graphics library, with custom pixel depth-dependent
  data packing, basic draw and font rendering functions
- `fonts/`: [serialized](font_compiler.py) [bitfont data](https://github.com/robhagemans/hoard-of-bitfonts)
- `hw/`: microcontroller HAL/embedded development library interfaces for
  SPI/UART GPIO operations
- `inst/`: display driver board-specific bytecode instructions & data
  formatting

Utilities, written in Python:

- [font_compiler.py](font_compiler.py): serializes bitfont data
- Backlog: color palette and bytecode generator utility
- Backlog: image processing utilities, for converting image files to
  library-friendly bitmap data (direct DROM support, like with fonts, plus
  SPIFFS support?)
- Backlog: canvas preview (export canvas data from C++ program, render in
  native window)

Some design notes:

- The canvas structure does bit packing, instead of splitting channels across
  multiple buffers, to try and reduce internal RAM read/write operations
- The canvas instantiation, draw, and GPIO operation APIs should be the same
  for every MCU-EPD configuration.
- There should be very little variation between EPD board APIs. Every board
  should at the very least have functions for an initialization sequence, a
  full refresh, and a partial refresh.
- Files in `fonts/`, `hw/`, and `inst/` should be header files. Even though
  ESP-IDF automatically discards unused symbols, other MCU toolchains might
  not.
- `hw/` and `inst/` should contain one file each for MCU/EPD types. For
  example, `hw/esp32idf.h` and `inst/heltec290bw.h`.
- Header files should use preprocessor directives to distinguish initialization
  and declaration. Sources should be initialized only once in one source file.
- Backlog: To save space, maybe wrap functions and other symbols in
  preprocessor directives? e.g. `#ifdef INIT_SYMBOL` or `#ifdef USE_SYMBOL`
- Backlog: Everything right now is written in C for purpose of compatibility.
  C++ wrappers should be offered for convenience.
- Backlog: Bytecode instructions and color palettes (BW, BWY, BWRY, etc.) per
  board should be generated from a common dictionary (preferably from Python,
  as the image processing utility will be).

To also add to the project repository:

- Image processing utilities (Python3?) for converting image files to
  library-friendly bitmap data

## TODO
- [x] Font parsing, header file creation
- [ ] Bit packing canvas data struct
  - [x] Set pixel
  - [x] Get pixel
  - [ ] Clear channel
  - [ ] Clear pixel
  - [ ] Batch set channel
  - [ ] Batch get channel
  - [ ] Batch clear channel
  - [ ] Batch clear pixels
- [ ] Color palette templating
- [ ] Image processing utilities (image files to bitmaps)
- [ ] Basic draw functions (use option structs?)
  - [ ] Line
    - [ ] Horizontal fast (x,y) origin to (dx,dy=0)
    - [ ] Vertical fast (x,y) origin to (dx=0,dy)
    - [ ] (x1,y1) to (x2,y2)
    - [ ] (x,y) origin to (dx,dy)
    - [ ] (x,y) origin to (r,θ)
  - [ ] Area fill
  - [ ] Clear screen
  - [ ] Clear region
  - [ ] Rectangle
  - [ ] Roundrect
  - [ ] Triangle
  - [ ] Text
  - [ ] Circle
  - [ ] Ellipse
  - [ ] Arc
  - [ ] Image
- [ ] Heltec 2.90" black/white display instruction set `inst/heltec_290_bw.h`
- [ ] ESP-IDF BSP support `hw/esp32idf.h`

Backlog:
- [ ] C++ wrappers
- [ ] Arduino BSP `hw/arduino.hpp`
