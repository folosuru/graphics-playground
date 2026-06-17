#!/bin/sh

OUPUT="out.png"
g++ ./src/main.cpp -o ./out -march=native -O5 -fopenmp\
  && rm -f $OUPUT \
  && ./out | ffmpeg -r 5 -f rawvideo -pixel_format rgb24 -video_size 320x180  -i - $OUPUT 2> /dev/null
