#!/usr/bin/env bash

[ ! -f build/$1 ] && echo "Executable build/$1 not found" && exit 1
[ ! -f utils/show_canvas.py ] && echo "Script utils/show_canvas.py not found" && exit 1

./build/$1 | ./utils/show_canvas.py
