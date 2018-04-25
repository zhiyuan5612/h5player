#include "h264bsd_decoder.h"
#include "libavcodec/avcodec.h"
#include <sys/time.h>


AVCodec ff_h264_decoder;
AVCodecParser ff_h264_parser;
	AVCodec *codec = NULL;
	AVCodecContext *codec_ctx =NULL;
	AVCodecParserContext* parser = NULL;
	AVFrame *frame = NULL;

	int frame_index = 0;
	AVPacket packet;
    uint8_t* yuvdata = NULL;
    int h = 0;


u32 h264bsdInit(storage_t *pStorage, u32 noOutputReordering)
{
    return 0;
}

u32 h264bsdDecode(storage_t *pStorage, u8 *byteStrm, u32 len, u32 picId,
    u32 *readBytes)
{
//	fprintf(stdout, "byteStrm len = %d\n",len);


    uint8_t* data = NULL;
    int size = 0;
    int bytes_used = av_parser_parse2(parser, codec_ctx, &data, &size, byteStrm, len, 0, 0, AV_NOPTS_VALUE);
//    fprintf(stdout, "av_parser_parse2 bytes_used=%d, data = %d,size =%d\n",bytes_used,data,size);
    *readBytes = bytes_used;
    av_init_packet(&packet);
    packet.data = data;
    packet.size = size;

	int got_frame = 0;
    int ret = avcodec_decode_video2(codec_ctx, frame, &got_frame, &packet);
 //   fprintf(stdout, "av_decode_video2 ret=%d, got = %d,w=%d,h=%d\n",ret,got_frame,frame->width,frame->height);
    if (ret < 0) {
    fprintf(stderr, "h264bsdNextOutputPicture ret=%d",ret);
    }

    if(got_frame == 0)
    {
        *readBytes = 0;
        return 0;
        
    }

    if(h==0)
    {
        h=1;
        *readBytes = 0;
        return 2;
    }
    
    return 1;
}

void h264bsdShutdown(storage_t *pStorage)
{


}

u8* h264bsdNextOutputPicture(storage_t *pStorage, u32 *picId, u32 *isIdrPic, u32 *numErrMbs)
{
    int wh = frame->width*frame->height;
	
   if(yuvdata==NULL)
   {
        yuvdata = (uint8_t*)malloc(wh*3/2);
        memset(yuvdata,1,wh*3/2);
   }
    
    uint8_t *p = yuvdata;
    memcpy(p,frame->data[0],wh);
    p+=wh;
    memcpy(p,frame->data[1],wh/4);
    p+=wh/4;
    memcpy(p,frame->data[2],wh/4);

    return yuvdata;
}

u32 h264bsdPicWidth(storage_t *pStorage)
{

    return frame->width;
}

u32 h264bsdPicHeight(storage_t *pStorage)
{

    return frame->height;
}

void h264bsdCroppingParams(storage_t *pStorage, u32 *croppingFlag,
    u32 *left, u32 *width, u32 *top, u32 *height)
{
    croppingFlag=0;


}


storage_t* h264bsdAlloc()
{


	avcodec_register(&ff_h264_decoder);
	av_register_codec_parser(&ff_h264_parser);
    
	codec = avcodec_find_decoder(AV_CODEC_ID_H264);
	if (!codec) {
		fprintf(stderr, "Codec not found\n");
		exit(1);
	}

	codec_ctx = avcodec_alloc_context3(codec);
	if (!codec_ctx) {
		fprintf(stderr, "Could not allocate video codec context\n");
		exit(1);
	}
	
	if (avcodec_open2(codec_ctx, codec, NULL) < 0) {
		fprintf(stderr, "Could not open codec\n");
		exit(1);
	}
	
	parser = av_parser_init(AV_CODEC_ID_H264);
	if(!parser) {
		fprintf(stderr, "Could not create H264 parser\n");
		exit(1);
	}

	frame = av_frame_alloc();
	if (!frame) {
		fprintf(stderr, "Could not allocate video frame\n");
		exit(1);
	}
		fprintf(stdout, "allocate video frame\n");
	
    return codec;
}

void h264bsdFree(storage_t *pStorage)
{


}
