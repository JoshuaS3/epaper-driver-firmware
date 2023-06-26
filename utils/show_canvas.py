#!/usr/bin/env python3

# File:
#   utils/show_canvas.py
# Description:
#   Uses matplotlib to produce a preview canvas with data piped from a C/C++
#   program.
#
# "epaper-driver-firmware" Copyright (c) 2023 Joshua Stockin
# <josh@joshstock.in> [https://joshstock.in] [https://github.com/JoshuaS3]
#
# This software is licensed and distributed under the terms of the MIT License.
# See the MIT License in the LICENSE file of this project's root folder.
#
# This comment block and its contents, including this disclaimer, MUST be
# preserved in all copies or distributions of this software's source.

import sys
import argparse
import matplotlib.pyplot as plt


# commonly used types
pixel_t = tuple[int, int, int]  # (R,G,B) 0-255
size_t = tuple[int, int]  # (x,y)
canvas_t = list[list[pixel_t]]  # 2D list of pixels


class Canvas:
    def __init__(self, size: size_t, pixel_depth: int):
        self.width = size[0]
        self.height = size[1]
        self.pixel_depth = pixel_depth
        # initialize canvas structure
        self.canvas = []
        for y in range(size[1]):
            self.canvas.append([])
            for x in range(size[0]):
                self.canvas[y].append(colorize(0))

    def set_pixel(self, x: int, y: int, rgb: pixel_t):
        if x >= self.width:
            return
        if y >= self.height:
            return
        self.canvas[y][x] = rgb


def read() -> bytearray:
    return sys.stdin.buffer.read()


def bits(buf: bytearray):
    x = []
    for b in buf:
        for i in range(8):
            x.append((b >> i) & 1)
    return x


def colorize(value):
    match value:
        case 0:
            return (255, 255, 255)
        case 1:
            return (0, 0, 0)
        case _:
            return (255, 255, 255)


def format(buf: bytearray, canvas_size: size_t, pixel_depth: int) -> canvas_t:
    canvas = Canvas(canvas_size, pixel_depth)
    bitarr = bits(buf)
    for i, v in enumerate(bitarr):
        x = i % canvas_size[0]
        y = i // canvas_size[0]
        canvas.set_pixel(x, y, colorize(v))
    return canvas


def plot(canvas: Canvas):
    plt.rcParams["axes.edgecolor"] = "black"
    plt.rcParams["axes.linewidth"] = 1
    plt.rcParams["xtick.top"] = True
    plt.rcParams["xtick.labeltop"] = True
    plt.rcParams["xtick.bottom"] = False
    plt.rcParams["xtick.labelbottom"] = False
    title = f"{canvas.width}x{canvas.height} canvas"
    plt.figure(num=title)
    img = plt.imshow(canvas.canvas)
    plt.title(title)
    plt.show()


if __name__ == "__main__":
    buf = read()
    canvas = format(buf, canvas_size=(296, 128), pixel_depth=1)
    plot(canvas)
