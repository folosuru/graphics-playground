#!/bin/sh

OUPUT="out.gif"
g++ ./src/main.cpp -o ./out -march=native -O3 -fopenmp\
  && rm -f $OUPUT \
  && ./out | ffmpeg -r 10 -f rawvideo -pixel_format rgb24 -video_size 320x180  -i - $OUPUT 2> /dev/null
