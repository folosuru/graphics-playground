#!/bin/sh


g++ ./src/main.cpp -o ./out -march=native -O5\
  && rm -f out.png \
  && ./out | ffmpeg -f rawvideo -pixel_format rgb24 -video_size 384x216  -i - ./out.png &> /dev/null
