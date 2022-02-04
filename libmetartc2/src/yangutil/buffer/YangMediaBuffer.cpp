﻿//
// Copyright (c) 2019-2022 yanggaofeng
//
#include <string.h>
#include <malloc.h>
#include <yangutil/buffer/YangMediaBuffer.h>
#include <yangutil/yangavinfotype.h>
#define Yang_MediaBuffer_Minus 5000
YangMediaBuffer::YangMediaBuffer() {
	resetIndex();
	m_cache_num = 5;
	m_mediaType = 1;
	m_uid = -1;
	m_frames = NULL;
	m_bufferManager = NULL;
	m_lock = PTHREAD_MUTEX_INITIALIZER;
	pthread_mutex_init(&m_lock,NULL);
}


YangMediaBuffer::~YangMediaBuffer() {
	if (m_frames) {
		for (int32_t i = 0; i < m_cache_num; i++) {
			yang_delete(m_frames[i]);
		}
		yang_deleteA(m_frames);
	}
	yang_delete(m_bufferManager);
}

void YangMediaBuffer::resetIndex() {
	m_putIndex = 0;
	m_getIndex = 0;
	m_nextIndex=0;

	m_ret=0;
	m_size = 0;
}
int32_t YangMediaBuffer::size(){

	return m_size;
}
void YangMediaBuffer::setSize(int padd) {
	pthread_mutex_lock(&m_lock);
	if (padd)
		m_size++;
	else
		m_size--;
	pthread_mutex_unlock(&m_lock);
}

void YangMediaBuffer::initFrames(int32_t pnum, int unitsize) {
	if (m_bufferManager == NULL)		m_bufferManager = new YangBufferManager(pnum, unitsize);
	if (m_frames == NULL) {
		m_frames = new YangFrame*[pnum];
		for (int32_t i = 0; i < pnum; i++) {
			m_frames[i] = new YangFrame();
			memset(m_frames[i],0,sizeof(YangFrame));
			m_frames[i]->payload = m_bufferManager->getBuffer();
		}
	}
}
void YangMediaBuffer::putFrame(YangFrame *pframe) {
	if (!pframe||size()>=m_cache_num)		return;

	if (m_putIndex >= m_cache_num||m_putIndex<0)		m_putIndex = 0;
	yang_frame_copy_buffer(pframe, m_frames[m_putIndex++]);
	setSize(1);
}
void YangMediaBuffer::getFrame(YangFrame *pframe) {
	if (!pframe||!size())		return;

	if (m_getIndex >= m_cache_num||m_getIndex<0)		m_getIndex = 0;
	yang_frame_copy_buffer(m_frames[m_getIndex++], pframe);
	setSize(0);


}
uint8_t* YangMediaBuffer::getFrameRef(YangFrame *pframe) {
	if (!size()||!pframe)		return NULL;

	if (m_getIndex >= m_cache_num||m_getIndex<0)		m_getIndex = 0;
	yang_frame_copy_nobuffer(m_frames[m_getIndex], pframe);
	uint8_t *p = m_frames[m_getIndex]->payload;
	m_getIndex++;
	setSize(0);
	return p;

}
YangFrame* YangMediaBuffer::getCurFrameRef() {
	if(!size()) return NULL;
    m_nextIndex=m_getIndex;
    if(m_nextIndex>=m_cache_num) m_nextIndex=0;
    return m_frames[m_nextIndex];
}

int64_t YangMediaBuffer::getNextFrameTimestamp(){
	if(!size()) return 0;
    m_nextIndex=m_getIndex;
    if(m_nextIndex>=m_cache_num) m_nextIndex=0;
    return m_frames[m_nextIndex]->pts;

}
