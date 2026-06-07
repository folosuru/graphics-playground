#!/bin/sh


g++ ./src/main.cpp -o ./out -march=native -O5\
  && rm -f out.png \
  && ./out | ffmpeg -framerate 5 -f rawvideo -pixel_format rgb24 -video_size 384x216  -i - ./out.avi &> /dev/null
