#!/bin/sh


g++ ./main.cpp -o ./out \
  && rm -f out.png \
  && ./out | ffmpeg -f rawvideo -pixel_format rgb24 -video_size 256x256  -i - ./out.png &> /dev/null
