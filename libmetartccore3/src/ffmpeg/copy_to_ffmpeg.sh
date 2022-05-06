#!/bin/bash

FFMPEG_DIR=~/code/av/ffmpeg-metartc/FFmpeg-n4.3.3
cp allformats.c $FFMPEG_DIR/libavformat/
cp demuxer_list.c $FFMPEG_DIR/libavformat/
cp ffmpeg.c $FFMPEG_DIR/fftools/
cp h264dec.c $FFMPEG_DIR/libavcodec/
cp muxer_list.c $FFMPEG_DIR/libavformat/
cp protocol_list.c $FFMPEG_DIR/libavformat/
cp protocols.c $FFMPEG_DIR/libavformat/
cp utils.c $FFMPEG_DIR/libavformat/
cp webrtc_demuxer.c $FFMPEG_DIR/libavformat/
cp webrtc_muxer.c $FFMPEG_DIR/libavformat/
cp webrtc_proto.c $FFMPEG_DIR/libavformat/
cp yang_h264_initExtra.h $FFMPEG_DIR/libavformat/
cp YangMetaConnection.h $FFMPEG_DIR/libavformat/
cp Makefile $FFMPEG_DIR/libavformat/
