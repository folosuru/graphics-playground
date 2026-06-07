#!/bin/sh


g++ ./src/main.cpp -o ./out -march=native -O5 -fopenmp\
  && rm -f out.png \
  && ./out | ffmpeg -r 5 -f rawvideo -pixel_format rgb24 -video_size 384x216  -i - ./out.gif &> /dev/null
