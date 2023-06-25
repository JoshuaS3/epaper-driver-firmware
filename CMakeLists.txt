# Compiles binaries for testing

cmake_minimum_required(VERSION 3.22)
project(ePaperDisplayTest)

include_directories(include)

set(SOURCES
    src/draw/canvas.c
    src/fonts/Courier10.c
    src/fonts/Courier12.c
    src/fonts/Courier15.c
    src/fonts/Helv8.c
    src/fonts/Helv10.c
    src/fonts/Helv12.c
    src/fonts/Helv14.c
    src/fonts/Helv18.c
    src/fonts/Helv24.c
    src/fonts/SansSerif8.c
    src/fonts/SansSerif10.c
    src/fonts/SansSerif12.c
    src/fonts/SansSerif14.c
    src/fonts/SansSerif18.c
    src/fonts/SansSerif24.c
    src/fonts/Serif8.c
    src/fonts/Serif10.c
    src/fonts/Serif12.c
    src/fonts/Serif14.c
    src/fonts/Serif18.c
    src/fonts/Serif24.c)

add_executable(basic_draw
                ${SOURCES}
                test/basic_draw.c)
