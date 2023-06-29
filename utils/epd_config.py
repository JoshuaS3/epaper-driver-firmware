#!/usr/bin/env python3

# File:
#   utils/epd_config.py
# Description:
#   Configuration and bytecode for ePaper displays.
#
# "epaper-driver-firmware" Copyright (c) 2023 Joshua Stockin
# <josh@joshstock.in> [https://joshstock.in] [https://github.com/JoshuaS3]
#
# This software is licensed and distributed under the terms of the MIT License.
# See the MIT License in the LICENSE file of this project's root folder.
#
# This comment block and its contents, including this disclaimer, MUST be
# preserved in all copies or distributions of this software's source.

EPD_COLORS = {
    "WHITE": {
        "Value": 0,
        "RGB": (255, 255, 255),
    },
    "BLACK": {
        "Value": 1,
        "RGB": (0, 0, 0),
    },
}

EPD_CONFIG = {
    "HELTEC_290_BW": {
        "Metadata": {
            "Vendor": "Heltec Automation",
            "ModelNumber": "HTE029A1",
        },
        "Capabilities": {
            "PixelWidth": 296,
            "PixelHeight": 128,
        },
        "ColorSupport": [EPD_COLORS["WHITE"], EPD_COLORS["BLACK"]],
        "Commands": {
            "",
        },
    },
}
