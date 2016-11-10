#! /bin/bash

VIDEO=$1
CODIGO=$2

echo "$CODIGO-640x360_750k.webm"

fmpeg -y -i $VIDEO -c:v libvpx-vp9 -keyint_min 150 -g 150 -tile-columns 4 -frame-parallel 1  -f webm -dash 1 -an -vf scale=160:190 -b:v 250k "$CODIGO-160x90_250k.webm"
ffmpeg -y -i "$CODIGO-160x90_250k.webm" -acodec copy -f segment -segment_time 10 -vcodec copy -reset_timestamps 1 -map 0  "$CODIGO-160x90_250k_%03d.webm" 

ffmpeg -y -i $VIDEO -c:v libvpx-vp9 -keyint_min 150 -g 150 -tile-columns 4 -frame-parallel 1  -f webm -dash 1 -an -vf scale=320:180 -b:v 500k "$CODIGO-320x180_500k.webm"
ffmpeg -y -i "$CODIGO-320x180_500k.webm" -acodec copy -f segment -segment_time 10 -vcodec copy -reset_timestamps 1 -map 0  "$CODIGO-320x180_500k_%03d.webm"

ffmpeg -y -i $VIDEO -c:v libvpx-vp9 -keyint_min 150 -g 150 -tile-columns 4 -frame-parallel 1  -f webm -dash 1 -an -vf scale=640:360 -b:v 750k "$CODIGO-640x360_750k.webm"
ffmpeg -y -i "$CODIGO-640x360_750k.webm" -acodec copy -f segment -segment_time 10 -vcodec copy -reset_timestamps 1 -map 0 "$CODIGO-640x360_750k_%03d.webm"
