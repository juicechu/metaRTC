#include <yangrtp/YangRtcpCompound.h>
#include <yangrtp/YangRtcpSR.h>
#include <yangrtp/YangRtcpNack.h>
#include <yangrtp/YangRtcpPli.h>
#include <yangrtp/YangRtcpRpsi.h>
#include <yangrtp/YangRtcpRR.h>
#include <yangrtp/YangRtcpSli.h>
#include <yangrtp/YangRtcpSR.h>
#include <yangrtp/YangRtcpTWCC.h>
#include <yangrtp/YangRtcpXr.h>
#include <yangutil/sys/YangLog.h>
#include <string.h>
#include <stdlib.h>
yang_vector_impl(YangRtcpCommon)

void yang_init_rtcpCompound(YangRtcpCompound* rtcp){
	if(rtcp==NULL) return;
	yang_init_YangRtcpCommonVector(&rtcp->rtcpVector);

}
void yang_destroy_rtcpCompound(YangRtcpCompound* rtcp){
	if(rtcp==NULL) return;
	yang_rtcpCompound_clear(rtcp);

}
int32_t yang_decode_rtcpCompound(YangRtcpCompound* rtcps,YangBuffer *buffer){
	if(rtcps==NULL) return 1;
	int32_t err = Yang_Ok;
	rtcps->data = buffer->data;
	rtcps->nb_data = buffer->size;

	    while (!yang_buffer_empty(buffer)) {
	        YangRtcpCommon* rtcp = (YangRtcpCommon*)calloc(1,sizeof(YangRtcpCommon));
	        YangRtcpHeader* header = (YangRtcpHeader*)(buffer->head);
	        if (header->type == YangRtcpType_sr) {
	        	yang_init_rtcpSR(rtcp);
	        	err=yang_decode_rtcpSR(rtcp,buffer);
	        } else if (header->type == YangRtcpType_rr) {
	        	yang_init_rtcpRR(rtcp);
	        	err=yang_decode_rtcpRR(rtcp,buffer);

	        } else if (header->type == YangRtcpType_rtpfb) {
	            if(1 == header->rc) {
	            	yang_init_rtcpNack(rtcp,0);
	                //nack
	            	yang_decode_rtcpNack(rtcp,buffer);

	            } else if (15 == header->rc) {
	                //twcc
	                //rtcp
	            }
	        } else if(header->type == YangRtcpType_psfb) {
	            if(1 == header->rc) {
	                // pli
	              	yang_init_rtcpPli(rtcp);
	            	err=yang_decode_rtcpPli(rtcp,buffer);
	            } else if(2 == header->rc) {
	                //sli

	            } else if(3 == header->rc) {
	                //rpsi
	               	yang_init_rtcpRpsi(rtcp);
	            	yang_decode_rtcpRpsi(rtcp,buffer);
	            } else {
	                // common psfb
	            	yang_init_rtcpPsfb(rtcp);
	            	err=yang_decode_rtcpPsfb(rtcp,buffer);
	            }
	        } else if(header->type == YangRtcpType_xr) {
	        	yang_init_rtcpXr(rtcp);
	        	err=yang_decode_rtcpXr(rtcp,buffer);
	        } else {
	        	yang_init_rtcpCommon(rtcp);
	        	err=yang_decode_rtcpCommon(rtcp,buffer);
	        }


	        if(err) {
	        	uint32_t header_type=rtcp->header.type;
	        	uint32_t header_rc=rtcp->header.rc;
	        	yang_free(rtcp);
	        	if(err==ERROR_RTC_RTCP_EMPTY_RR)  continue;
	        	return yang_error_wrap(err, "decode rtcp type=%u rc=%u", header_type, header_rc);
	   	    }
	        yang_rtcpCompound_add_rtcp(rtcps,rtcp);
	        yang_free(rtcp);

	    }

	    return err;
}
int32_t yang_encode_rtcpCompound(YangRtcpCompound *rtcps, YangBuffer *buffer) {
	if(rtcps==NULL) return 1;
	int32_t err = Yang_Ok;
	if (!yang_buffer_require(buffer, rtcps->nb_bytes)) {
		return yang_error_wrap(ERROR_RTC_RTCP, "requires %d bytes",
				rtcps->nb_bytes);
	}
	for (int i = 0; i < rtcps->rtcpVector.vlen; i++) {
		YangRtcpCommon *rtcp = &rtcps->rtcpVector.payload[i];
		if (rtcp->header.type == YangRtcpType_sr) {
			err = yang_encode_rtcpSR(rtcp, buffer);
		} else if (rtcp->header.type == YangRtcpType_rr) {
			err = yang_encode_rtcpRR(rtcp, buffer);
		} else if (rtcp->header.type == YangRtcpType_rtpfb) {
			if (1 == rtcp->header.rc) {
				//nack

				err=yang_encode_rtcpNack(rtcp,buffer);
			} else if (15 == rtcp->header.rc) {
				//twcc

			}
		} else if (rtcp->header.type == YangRtcpType_psfb) {
			if (1 == rtcp->header.rc) {
				// pli
				err = yang_encode_rtcpPli(rtcp, buffer);
			} else if (2 == rtcp->header.rc) {
				//sli

			} else if (3 == rtcp->header.rc) {
				//rpsi
				err=yang_encode_rtcpRpsi(rtcp,buffer);
			} else {
				// common psfb
				err = yang_encode_rtcpPsfb(rtcp, buffer);
			}
		} else if (rtcp->header.type== YangRtcpType_xr) {
			err = yang_encode_rtcpXr(rtcp, buffer);
		} else {
			err = yang_encode_rtcpCommon(rtcp, buffer);
		}

	}
	yang_rtcpCompound_clear(rtcps);

	return err;
}
uint64_t yang_rtcpCompound_nb_bytes(YangRtcpCompound *rtcps){
	return 1500;
}

int32_t yang_rtcpCompound_add_rtcp(YangRtcpCompound *rtcps, YangRtcpCommon *rtcp)
{
	if(rtcps==NULL||rtcp==NULL) return 1;
	yang_insert_YangRtcpCommonVector(&rtcps->rtcpVector,rtcp);
    return Yang_Ok;
}
void yang_rtcpCompound_clear(YangRtcpCompound *rtcps)
{
	if(rtcps==NULL) return ;
	if(rtcps->rtcpVector.vlen){
		for(int i=0;i<rtcps->rtcpVector.vlen;i++){
			yang_destroy_rtcpCommon(&rtcps->rtcpVector.payload[i]);
		}
		yang_clear_YangRtcpCommonVector(&rtcps->rtcpVector);
	}

}

