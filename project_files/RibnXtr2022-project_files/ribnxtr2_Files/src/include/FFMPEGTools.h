#ifndef	FFMPEGTOOLS_H
#define	FFMPEGTOOLS_H

#include <qwidget.h>

#include <math.h>
#include <stdlib.h>

#include "RawDataSet.h"
#include "OtherCommonFunctions.h"


extern "C"
{
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"

//
#include "libavutil/mathematics.h"
#include "libavutil/opt.h"
#include "libavutil/rational.h"
#include "libavutil/avstring.h"
#include "libswscale/swscale.h"
#include "libavutil/rational.h"
}
#define INBUF_SIZE 4096
class StereoViewWidget;

class FFMPEGTools
{

	public:
		FFMPEGTools();
		
		
		
		~FFMPEGTools();

		bool	isRecordInProgress(){return recordInProgress;};

		int getFrameCount(){return frameCount;};
		void	initVariablesForRecord(const QString &filename, int width, int height, AVCodecID mode, int bitrate,int framerate);
		void	recordFrame(QImage *img);
		void	freeVariablesForRecord();

		RawDataSet *loadVideoAsRawDataSet (const QString &filename);
		void		saveRawDataSetAsVideo(RawDataSet *data,const QString &filename);

		bool recordInProgress;

private:

		int validWidth, validHeight;
		AVFormatContext *pFormatCtx ;
		AVCodecContext  *pCodecCtx ;

		AVOutputFormat *avOutputFormat;
		AVFormatContext *avFormatContext ;
		unsigned char *codecBuf;
		int codecBufSize;

		AVStream *avStream;

		AVCodec         *pCodec ;
		AVFrame         *pFrame ; 
		AVFrame         *pFrameRGB ;
		AVPacket        packet;
		uint8_t         *buffer ;
		AVDictionary    *optionsDict ;
		struct SwsContext      *sws_ctx ;
		
		SwsContext *img_convert_ctx;
		unsigned char *img_bgra;
		int out_size;
		
		AVCodecContext *c;
		int outbuf_size;
		uint8_t *outbuf, *picture_buf;
		FILE *f;
		AVFrame *picture;
		int frameCount;


	
};

#endif //FFMPEGTOOLS_H
