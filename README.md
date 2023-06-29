# ESP-IDF driver for 2.90" BW Heltec E-Ink display

**Not functional yet, do not use.**

Eventually this will be a working ESP-IDF driving component for Heltec 2.90" BW
ePaper displays (EPDs). I'm planning on future-proofing this as much as
possible, so it could become a general use EPD component for virtually any
configuration of MCU and EPD. See the functional diagram UML for details.

## Project structure

- `include/`: header files
- `src/draw/`: contains canvas structure, graphics library
- `src/hw/`: microcontroller HAL-dependent implementations for SPI/UART GPIO
  operations
- `src/fonts/`: [serialized](font_compiler.py) [bitfont data](https://github.com/robhagemans/hoard-of-bitfonts)
- `src/inst/`: display-specific bytecode instructions
- `test/`: integration, unit tests
- `utils/`: Python scripts, hardware configurations
- `CMakeLists.txt`: **development build system for tests, should be removed or
  replaced when using**

`fonts/` and `inst/` should be largely generated code.

Utilities, written in Python:

- [x] [font_compiler.py](font_compiler.py): serializes bitfont data
- [ ] color palette and bytecode generator utility
- [x] canvas preview (export canvas data from test program, render in native
      window)
- [ ] image processing utilities, for converting image files to library-friendly
      bitmap data
  - [ ] direct DROM support, like with fonts
  - [ ] SPIFFS support?

Some design notes:

- The canvas structure does bit packing, instead of splitting channels across
  multiple buffers, to try and reduce internal RAM usage
- The canvas instantiation, draw, and GPIO operation APIs should be the same for
  every MCU-EPD configuration.
- There should be very little variation between EPD board APIs. Every board
  should at the very least have functions for an initialization sequence, a full
  refresh, and a partial refresh.

## TODO
- [x] Font parsing, header file creation
- [ ] Bit packing canvas data struct
  - [x] Set pixel
  - [x] Get pixel
  - [ ] Clear pixel
  - [ ] Clear channel
  - [ ] Batch set channel
  - [ ] Batch get channel
  - [ ] Batch clear channel
  - [ ] Batch clear pixels
- [ ] Color palette templating
- [ ] Image processing utilities (image files to bitmaps)
- [ ] Basic draw functions (use option structs?)
  - [ ] Line
    - [x] Horizontal fast (x,y) origin to (dx,dy=0)
    - [x] Vertical fast (x,y) origin to (dx=0,dy)
    - [x] (x1,y1) to (x2,y2)
    - [x] (x,y) origin to (dx,dy)
    - [ ] (x,y) origin to (r,θ)
  - [ ] Area fill
  - [ ] Clear screen
  - [ ] Clear region
  - [ ] Rectangle
  - [ ] Roundrect
  - [ ] Triangle
  - [ ] Text
  - [x] Circle
  - [ ] Ellipse
  - [ ] Arc
  - [ ] Image
- [ ] Heltec 2.90" black/white display instruction set `src/epd/heltec_290_bw.h`
- [ ] ESP-IDF BSP support `src/mcu/esp32idf.h`
  - [ ] ESP-IDF component CMakeLists.txt

Backlog:
- [ ] C++ wrappers
- [ ] Arduino HAL `hw/arduino.hpp`
