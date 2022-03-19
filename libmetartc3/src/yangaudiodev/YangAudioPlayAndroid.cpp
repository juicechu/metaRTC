﻿//
// Copyright (c) 2019-2022 yanggaofeng
//
#include <yangaudiodev/YangAudioPlayAndroid.h>
#ifndef _WIN32
#ifdef __ANDROID__
#include <yangutil/yang_unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <stdio.h>
#include <yangutil/sys/YangLog.h>
YangAudioPlayAndroid::YangAudioPlayAndroid(YangContext *pcontext):YangAudioPlay(pcontext){

	m_loops = 0;

	m_frames = 0;
	m_isStart = 0;
	m_contextt = 0;
}

YangAudioPlayAndroid::~YangAudioPlayAndroid() {

	closeAudio();
}

int YangAudioPlayAndroid::init() {
	if (m_contextt == 1)
		return Yang_Ok;

	m_frames = m_context->avinfo.audio.sample / 50;

	uint32_t val = 0;
	int32_t dir = 0;

	m_audioData.initRender(m_sample,m_channel);
	m_contextt = 1;
	return Yang_Ok;

}

void YangAudioPlayAndroid::closeAudio() {



}

void YangAudioPlayAndroid::stopLoop() {
	m_loops = 0;
}

void YangAudioPlayAndroid::startLoop() {

	m_loops = 1;
	unsigned long status = 0;
	uint8_t *pcm = new uint8_t[4096*2];
	memset(pcm,0,4096*2);
	YangAutoFreeA(uint8_t,pcm);
	uint8_t *tmp = NULL;

	tmp = NULL;


}
#endif
#endif
