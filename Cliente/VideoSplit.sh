#! /bin/bash

VIDEO=$1
CODIGO=$2

ffmpeg -y -i $VIDEO -c:v libvpx-vp9 -keyint_min 150 -g 150 -tile-columns 4 -frame-parallel 1  -f webm -dash 1 -an -vf scale=160:190 -b:v 250k $CODIGO_160x90_250k.webm
ffmpeg -y -i $CODIGO_160x90_250k.webm -acodec copy -f segment -segment_time 10 -vcodec copy -reset_timestamps 1 -map 0  $CODIGO_160x90_250k-%03d.webm 

ffmpeg -y -i $VIDEO -c:v libvpx-vp9 -keyint_min 150 -g 150 -tile-columns 4 -frame-parallel 1  -f webm -dash 1 -an -vf scale=320:180 -b:v 500k $CODIGO_320x180_500k.webm
ffmpeg -y -i $CODIGO_320x180_500k.webm -acodec copy -f segment -segment_time 10 -vcodec copy -reset_timestamps 1 -map 0  $CODIGO_320x180_500k-%03d.webm

ffmpeg -y -i $VIDEO -c:v libvpx-vp9 -keyint_min 150 -g 150 -tile-columns 4 -frame-parallel 1  -f webm -dash 1 -an -vf scale=640:360 -b:v 750k  $CODIGO_640x360_750k.webm
ffmpeg -y -i $CODIGO_640x360_750k.webm -acodec copy -f segment -segment_time 10 -vcodec copy -reset_timestamps 1 -map 0  $CODIGO_640x360_750k-%03d.webm
