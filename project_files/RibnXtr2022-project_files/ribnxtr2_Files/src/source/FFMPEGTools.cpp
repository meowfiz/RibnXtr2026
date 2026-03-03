#include "FFMPEGTools.h"

FFMPEGTools::FFMPEGTools()
{
	recordInProgress = false;
	pFrame = NULL;
	img_bgra = NULL;
	img_convert_ctx = NULL;
	validWidth = 0;
	validHeight = 0;
	optionsDict = NULL;
//	codecBuf = NULL;
}
//--------------------------

FFMPEGTools::~FFMPEGTools()
{

}
//---------------------------
void	FFMPEGTools::initVariablesForRecord(const QString& filename, int width, int height, AVCodecID mode, int bitrate, int framerate)
{
	if (!filename.isEmpty())
	{
		av_register_all();
		validWidth = (width % 8 == 0) ? width : width + 8 - width % 8; //powiekszam do nastepnej wielokrotnosci 8
		validHeight = (height % 8 == 0) ? height : height + 8 - height % 8; //powiekszam do nastepnej wielokrotnosci 8

		avFormatContext = avformat_alloc_context();
		avOutputFormat = av_guess_format("avi", NULL, NULL);

		if (!this->avOutputFormat) { cout << "ERROR avOutputFormat";		return; }
		AVCodecID id;
		id = mode;
		avOutputFormat->video_codec = mode;
		avFormatContext->oformat = avOutputFormat;
		strcpy(this->avFormatContext->filename, filename.toStdString().c_str());
		avStream = avformat_new_stream(avFormatContext, 0);
		if (!avStream) { cout << "ERROR avStream";		return; }

		c = this->avStream->codec;
		c->codec_id = (AVCodecID)this->avOutputFormat->video_codec;
		c->codec_type = AVMEDIA_TYPE_VIDEO;
		c->width = validWidth;
		c->height = validHeight;
		c->pix_fmt = AV_PIX_FMT_YUV420P;//PIX_FMT_YUVJ422P;
		c->time_base.den = framerate;
		c->time_base.num = 1;




		//AVRational ar;
		//ar.den = 25;
		//ar.num = 1;
		//c->time_base = ar;
		//c->time_base = { 1, 25 };
		avStream->time_base = c->time_base;
		c->gop_size = 10; /* emit one intra frame every ten frames */
		c->max_b_frames = 1;
		if (bitrate < 3)
		{
			switch (bitrate)
			{
			case 0:
				c->bit_rate = 3 * 1024 * 1024;
				break;
			case 1:
				c->bit_rate = 6 * 1024 * 1024;
				break;
			case 2:
				c->bit_rate = 12 * 1024 * 1024;
				break;
			}
		}
		else
		{
			c->bit_rate = bitrate;
		}



		c->bit_rate_tolerance = c->bit_rate;

		AVCodec* codec = avcodec_find_encoder(c->codec_id);
		if (!codec) { cout << "ERROR codec";		return; }
		if (avcodec_open2(c, codec, &optionsDict) < 0) { cout << "ERROR avcodec_open";		return; }
		//codecBufSize = 2 * c->width * c->height * 4 * 2;
		//codecBuf = new unsigned char[codecBufSize];
		//if (!codecBuf) { cout << "ERROR codecBuf";		return; }
		picture = av_frame_alloc();
		if (!picture) { cout << "ERROR picture";		return; }

		int size = validWidth * validHeight;
		picture_buf = (uint8_t*)malloc((size * 3) / 2 * 2); /* size for YUV 420 */

		picture->data[0] = picture_buf;
		picture->data[1] = picture->data[0] + size;
		picture->data[2] = picture->data[1] + size / 4;
		picture->linesize[0] = validWidth;
		picture->linesize[1] = validWidth / 2;
		picture->linesize[2] = validWidth / 2;
		picture->width = c->width;
		picture->height = c->height;
		picture->format = c->pix_fmt;


		

		//	pFrame=avcodec_alloc_frame();

		if (avio_open(&avFormatContext->pb, avFormatContext->filename, AVIO_FLAG_WRITE) < 0) { cout << "ERROR avio_open";		return; }
		avformat_write_header(avFormatContext, NULL);

		outbuf_size = avpicture_get_size(c->pix_fmt, c->width, c->height);

		outbuf = (uint8_t*)malloc(outbuf_size);

		/*
		{25, "CelB",       AVMEDIA_TYPE_VIDEO,   CODEC_ID_NONE, 90000, -1},
		//00063   {26, "JPEG",       AVMEDIA_TYPE_VIDEO,   CODEC_ID_MJPEG, 90000, -1},
		//00064   {28, "nv",         AVMEDIA_TYPE_VIDEO,   CODEC_ID_NONE, 90000, -1},
		//00065   {31, "H261",       AVMEDIA_TYPE_VIDEO,   CODEC_ID_H261, 90000, -1},
		//00066   {32, "MPV",        AVMEDIA_TYPE_VIDEO,   CODEC_ID_MPEG1VIDEO, 90000, -1},
		//00067   {32, "MPV",        AVMEDIA_TYPE_VIDEO,   CODEC_ID_MPEG2VIDEO, 90000, -1},
		//00068   {33, "MP2T",       AVMEDIA_TYPE_DATA,    CODEC_ID_MPEG2TS, 90000, -1},
		//00069   {34, "H263",       AVMEDIA_TYPE_VIDEO,   CODEC_ID_H263, 90000, -1},
		//*/
		//AVDictionary    **optionsDict = NULL;
		//AVCodec *codec;
		//c= NULL;
		//int i,j, out_size, size, x, y;
		////FILE *f;
		//


		//printf("Video encoding\n");

		///* find the mpeg1 video encoder */
		//codec = avcodec_find_encoder(CODEC_ID_MPEG4);
		//if (!codec) {
		//	fprintf(stderr, "codec not found\n");
		//	exit(1);
		//}

		//c= avcodec_alloc_context3(codec);
		//picture= avcodec_alloc_frame();

		///* put sample parameters */
		//c->bit_rate = bitrate;
		///* resolution must be a multiple of two */
		//c->width = validWidth;
		//c->height = validHeight;
		////  AVRational aspect;
		////  aspect.den=data->GetYscale()*1000;
		////aspect.num=data->GetXscale()*1000;

		////c->sample_aspect_ratio=aspect;
		////c->priv
		///* frames per second */
		//AVRational ar;
		//ar.den=25;
		//ar.num=1;
		//c->time_base= ar;
		//c->gop_size = 10; /* emit one intra frame every ten frames */
		//c->max_b_frames=1;
		//c->pix_fmt = PIX_FMT_YUV420P;
		//// c->bit_rate_tolerance=10000;
		///* open it */

		////**********************
		////trick na voxelsizes?

		////c->lumi_masking=data->GetXscale();
		////c->temporal_cplx_masking=data->GetYscale();
		////c->spatial_cplx_masking=data->GetZscale();

		////**********************


		//if (avcodec_open2(c, codec,optionsDict) < 0) {
		//	fprintf(stderr, "could not open codec\n");
		//	exit(1);
		//}

		//f = fopen(filename.toStdString().c_str(), "wb");
		//if (!f) {
		//	fprintf(stderr, "could not open %s\n", filename);
		//	exit(1);
		//}




		///* alloc image and output buffer */
		//outbuf_size = 1000000;
		//outbuf = (uint8_t*)malloc(outbuf_size);
		//size = validWidth * validHeight; //input
		//picture_buf = (uint8_t*)malloc((size * 3) / 2); /* size for YUV 420 */

		//picture->data[0] = picture_buf;
		//picture->data[1] = picture->data[0] + size;
		//picture->data[2] = picture->data[1] + size / 4;
		//picture->linesize[0] = validWidth;
		//picture->linesize[1] = validWidth / 2;
		//picture->linesize[2] = validWidth / 2;


		//bgra

		//img_bgra=new unsigned char[width*height];
		/*uint8_t *srcplanes[3];
		srcplanes[0]=(uint8_t*)img_bgra;
		srcplanes[1]=0;
		srcplanes[2]=0;
		int srcstride[3];
		srcstride[0]=width*4;
		srcstride[1]=0;
		srcstride[2]=0;*/

		/*	double scaler=1.0;*/
			/*int sliceSize=c->width*c->height;
			if (data->GetMaxValue()>255)
			{
				scaler=255.0/data->GetMaxValue();
			}
			unsigned short *tab=data->GetDataArray();
			unsigned char val;
			int pos;
			int r,g,b,a;
			int pos2;*/
		recordInProgress = true;
		frameCount = 0;

	}
}
//----------------------------------
void	FFMPEGTools::recordFrame(QImage* img)
{
	int width = img->width();
	int height = img->height();
	int validWidthTmp = (width % 8 == 0) ? width : width + 8 - width % 8; //powiekszam do nastepnej wielokrotnosci 8
	int validHeightTmp = (height % 8 == 0) ? height : height + 8 - height % 8; //powiekszam do nastepnej wielokrotnosci 8

//	if ( (c->width==img->width()) && (c->height==img->height())&&(f!=NULL))
	if ((validWidthTmp == validWidth) && (validHeightTmp == validHeight))
	{

		fflush(stdout);

		//preapre img
		AVCodecContext* cc = avStream->codec;

		uint8_t* srcplanes[3];
		srcplanes[0] = (uint8_t*)img->bits();
		srcplanes[1] = 0;
		srcplanes[2] = 0;
		int srcstride[3];
		srcstride[0] = width * 4;
		srcstride[1] = 0;
		srcstride[2] = 0;
		img_convert_ctx = sws_getCachedContext(img_convert_ctx, width, height, AV_PIX_FMT_BGRA, c->width, c->height, AV_PIX_FMT_YUV420P, SWS_BICUBIC, NULL, NULL, NULL);

		sws_scale(img_convert_ctx, srcplanes, srcstride, 0, height, picture->data, picture->linesize);



		/*	AVPacket pkt;
			av_init_packet(&pkt);
			 pkt.data = NULL;
			pkt.size = 0;
	*/
	/* encode the image */
		int frame_finished = 0;
		int toAdd;
		//	out_size = avcodec_encode_video(c, outbuf, outbuf_size, picture);
		//	toAdd = avcodec_encode_video(c, codecBuf, codecBufSize, picture);






			//-out_size =
			//	-avcodec_encode_video(c,
			//		-p->video_outbuf,
			//		-p->video_outbuf_size, picture_ptr);

		AVPacket pkt2;
		int got_packet = 0;
		av_init_packet(&pkt2);
		pkt2.data = outbuf;
		pkt2.size = outbuf_size;

		out_size = avcodec_encode_video2(c, &pkt2, picture, &got_packet);

		if (!out_size && got_packet && c->coded_frame)
		{
			c->coded_frame->pts = pkt2.pts;
			c->coded_frame->key_frame = !!(pkt2.flags & AV_PKT_FLAG_KEY);
			if (c->codec->capabilities & AV_CODEC_CAP_INTRA_ONLY)
				c->coded_frame->pict_type = AV_PICTURE_TYPE_I;
		}

		if (pkt2.side_data_elems > 0)
		{
			int i;
			for (i = 0; i < pkt2.side_data_elems; i++)
				av_free(pkt2.side_data[i].data);
			av_freep(&pkt2.side_data);
			pkt2.side_data_elems = 0;
		}

		if (!out_size)
			toAdd = pkt2.size;








		//avcodec_encode_video2(c, &pkt, picture, &frame_finished);
		if (toAdd)
		{
			AVPacket pkt;
			av_init_packet(&pkt);

			//to do playback at actual recorded rate, this will need more work
			pkt.pts = cc->coded_frame->pts;
			//pkt.dts = ?; not dure what decompression time stamp should be
			pkt.data = outbuf;
			pkt.size = toAdd;
			pkt.stream_index = this->avStream->index;
			if (cc->coded_frame->key_frame) { pkt.flags |= AV_PKT_FLAG_KEY; }
			pkt.duration = 0; //presentation duration in time_base units or 0 if NA
			pkt.pos = -1; //byte position in stream or -1 if NA

			toAdd = av_write_frame(avFormatContext, &pkt);
			if (toAdd) { cout << "ERROR toAdd";		return; }

		}

		//out_size=avcodec_encode_video2(c,
		//printf("encoding frame %3d (size=%5d)\n", i, out_size);
		//if (frame_finished)
		{
			//	fwrite(outbuf, 1, out_size, f);
			frameCount++;
		}
	}
	else
	{
		qDebug("bad frame size stopping!");
		freeVariablesForRecord();
	}
}
//---------------------------------------
void	FFMPEGTools::freeVariablesForRecord()
{
	//	delete []img_bgra;
	/* get the delayed frames */
	//int i=0;

	//
	///*	AVPacket pkt;
 //       av_init_packet(&pkt);
	//	 pkt.data = NULL;
 //       pkt.size = 0;*/

	//	/* encode the image */
	//	int frame_finished=0;

	//for(; out_size; i++)
	//{
	//		fflush(stdout);

	//	//avcodec_encode_video2(c, &pkt, picture, &frame_finished);

	//		out_size = avcodec_encode_video(c, outbuf, outbuf_size, NULL);
	//	//if (frame_finished)
	//	{
	//		printf("write frame %3d (size=%5d)\n", i, out_size);
	//		fwrite(outbuf, 1, out_size, f);
	//	}
	//}

	///* add sequence end code to have a real mpeg file */
	//outbuf[0] = 0x00;
	//outbuf[1] = 0x00;
	//outbuf[2] = 0x01;
	//outbuf[3] = 0xb7;
	//fwrite(outbuf, 1, 4, f);
	////av_write_trailer(c);
	//fclose(f);
	//free(picture_buf);
	//free(outbuf);

	//avcodec_close(c);
	//av_free(c);
	//av_free(picture);
	//printf("\n");

	//if (picture_buf)
	//{
	//	av_free(picture_buf);
	//	picture_buf=NULL;
	//
	//}
	
	/*if (codecBuf != NULL)
	{
		free(codecBuf);
		codecBuf = NULL;
	}*/

	qDebug("closing");
	if (avFormatContext)
	{
		if (recordInProgress == true)
		{
			av_write_trailer(avFormatContext);
			avio_close(avFormatContext->pb);
			recordInProgress = false;
		}
		av_free(avFormatContext);
		avFormatContext = 0;
	}
	qDebug("avFormatContext - done");
	if (avStream)
	{
		av_free(avStream);
		avStream = NULL;
	}
	qDebug("avStream - done");
	if (avOutputFormat)
	{
		avOutputFormat = 0;
	}
	qDebug("avOutputFormat - done");
	if (picture_buf != NULL) free(picture_buf);
	qDebug("picture_buf - done");
	if (picture != NULL)		av_free(picture);
	qDebug("picture - done");
}
//--------------------------------------------
RawDataSet* FFMPEGTools::loadVideoAsRawDataSet(const QString& filename)
{

	if (!filename.isEmpty())
	{
		int             i, videoStream;
		int             frameFinished;
		int             numBytes;

		pFormatCtx = NULL;

		pCodecCtx = NULL;
		pCodec = NULL;
		pFrame = NULL;
		pFrameRGB = NULL;


		uint8_t* buffer = NULL;

		optionsDict = NULL;
		sws_ctx = NULL;

		/* if(argc < 2) {
		printf("Please provide a movie file\n");
		return -1;
		}*/
		// Register all formats and codecs
		av_register_all();

		// Open video file
		if (avformat_open_input(&pFormatCtx, filename.toStdString().c_str(), NULL, NULL) != 0)
			return NULL;//-1; // Couldn't open file

		// Retrieve stream information
		if (avformat_find_stream_info(pFormatCtx, NULL) < 0)
			return NULL;//-1; // Couldn't find stream information

		// Dump information about file onto standard error
		av_dump_format(pFormatCtx, 0, filename.toStdString().c_str(), 0);

		// Find the first video stream
		videoStream = -1;
		for (i = 0; i < pFormatCtx->nb_streams; i++)
			if (pFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO) {
				videoStream = i;
				break;
			}
		if (videoStream == -1)
			return NULL;//-1; // Didn't find a video stream

		// Get a pointer to the codec context for the video stream
		pCodecCtx = pFormatCtx->streams[videoStream]->codec;

		// Find the decoder for the video stream
		pCodec = avcodec_find_decoder(pCodecCtx->codec_id);
		if (pCodec == NULL) {
			fprintf(stderr, "Unsupported codec!\n");
			return NULL;//-1; // Codec not found
		}
		// Open codec
		if (avcodec_open2(pCodecCtx, pCodec, &optionsDict) < 0)
			return NULL;//-1; // Could not open codec

		// Allocate video frame
		pFrame = av_frame_alloc();

		// Allocate an AVFrame structure
		pFrameRGB = av_frame_alloc();
		if (pFrameRGB == NULL)
			return NULL;//-1;

		// Determine required buffer size and allocate buffer
		numBytes = avpicture_get_size(AV_PIX_FMT_RGB24, pCodecCtx->width,
			pCodecCtx->height);
		buffer = (uint8_t*)av_malloc(numBytes * sizeof(uint8_t));

		sws_ctx =
			sws_getContext
			(
				pCodecCtx->width,
				pCodecCtx->height,
				pCodecCtx->pix_fmt,
				pCodecCtx->width,
				pCodecCtx->height,
				AV_PIX_FMT_RGB24,
				SWS_BILINEAR,
				NULL,
				NULL,
				NULL
			);

		// Assign appropriate parts of buffer to image planes in pFrameRGB
		// Note that pFrameRGB is an AVFrame, but AVFrame is a superset
		// of AVPicture
		avpicture_fill((AVPicture*)pFrameRGB, buffer, AV_PIX_FMT_RGB24,
			pCodecCtx->width, pCodecCtx->height);

		// Read frames and save first five frames to disk
		i = 0;

		int count = 0;
		int w = pCodecCtx->width;
		int h = pCodecCtx->height;

		while (av_read_frame(pFormatCtx, &packet) >= 0) {
			//if(packet.stream_index==videoStream) {
			//  // Decode video frame
			//  avcodec_decode_video2(pCodecCtx, pFrame, &frameFinished, 
			//  &packet);
			// 
			//  // Did we get a video frame?
			//  if(frameFinished) {

			/*	if  (count==0)
			{
			w = pCodecCtx->width;
			h = pCodecCtx->height;
			}*/
			count++;
			/* }}*/
		}
		//	//w=839;
		//	//h=686;
		//	//count=256;
		int mm = INT_MAX;

		double scale[3] = { 1,1,1 };
		/*if ((pCodecCtx->lumi_masking>0)&&( pCodecCtx->temporal_cplx_masking>0)&&( pCodecCtx->spatial_cplx_masking>0))
		{
		scale[0]=pCodecCtx->lumi_masking;
		scale[1]=pCodecCtx->temporal_cplx_masking;
		scale[2]=pCodecCtx->spatial_cplx_masking;
		}*/
		//pCodecCtx
		//				 c->lumi_masking=data->GetXscale();
		//	 c->temporal_cplx_masking=data->GetYscale();
		//	 c->spatial_cplx_masking=data->GetZscale();


		RawDataSet* tData = new RawDataSet(w, h, count, 1, 1, 1);
		tData->ZeroQ();
		/*for(i=0;i<w*h*count;i++)
		{

		tData->GetDataArray()[i]=255;
		}*/
		tData->initRGBArrays();


		//unsigned char *pixeldataV=(unsigned char*)tData->GetScalarPointer();
		unsigned short* dataR = tData->GetDataArray();
		unsigned char* dataG = tData->GetGChannelUCHARArray();
		unsigned char* dataB = tData->GetBChannelUCHARArray();

		int frame = 0;
		int j;
		int step1, step2;
		unsigned char r, g, b;
		unsigned long pos = 0;
		unsigned char* pixeldataV;



		if (avformat_seek_file(pFormatCtx, videoStream, 0, 0, 0, AVSEEK_FLAG_FRAME) < 0)
			return NULL;
		int cc = 0;
		while (av_read_frame(pFormatCtx, &packet) >= 0) {
			// Is this a packet from the video stream?
			if (packet.stream_index == videoStream) {
				// Decode video frame
				int trials = 0;
				do {
					avcodec_decode_video2(pCodecCtx, pFrame, &frameFinished,
						&packet);

					trials++;
				} while ((frameFinished == false) && (trials < 2000));
				// Did we get a video frame?
				if (frameFinished) {
					// Convert the image from its native format to RGB
					sws_scale
					(
						sws_ctx,
						(uint8_t const* const*)pFrame->data,
						pFrame->linesize,
						0,
						pCodecCtx->height,
						pFrameRGB->data,
						pFrameRGB->linesize
					);
					cc++;
					step2 = 0;//(w*(h-1))*3;
					if (cc == 250)
					{
						i = i;
						//pos=0;
					}
					if (pFrameRGB != NULL)
					{
						pixeldataV = (unsigned char*)pFrameRGB->data[0];
						for (j = 0; j < h; j++)
						{
							for (i = 0; i < w; i++)
							{
								r = pixeldataV[step2];
								g = pixeldataV[step2 + 1];
								b = pixeldataV[step2 + 2];
								step2 += 3;
								dataR[pos] = r;
								dataG[pos] = g;
								dataB[pos] = b;
								pos++;
							}

						}
					}
					else
					{
						i = i;
					}
					//}
					// Save the frame to disk
					/*if(++i<=5)
					SaveFrame(pFrameRGB, pCodecCtx->width, pCodecCtx->height ,
					i);*/
				}
				else
				{
					cout << " problem with " << cc << "frame\n";
				}
			}
			else
			{
				i = i;
			}

			// Free the packet that was allocated by av_read_frame
			av_free_packet(&packet);
		}

		// Free the RGB image
		av_free(buffer);
		av_free(pFrameRGB);

		// Free the YUV frame
		av_free(pFrame);

		// Close the codec
		avcodec_close(pCodecCtx);

		// Close the video file
		avformat_close_input(&pFormatCtx);

		QString tt = filename;
		/* if(tt.mid(tt.length()-6,6)==".rdata")
		tt.remove(tt.length()-6,6);


		RawDataSetName=tt;*/

		int	pp = tt.lastIndexOf("/");
		if (pp >= 0)
			tt.remove(0, pp + 1);

		tData->SetRawDataSetName(tt);
		return tData;
		// InsertRawDataSet(tData);
	}
	return NULL;
}
//-----------------------------
void		FFMPEGTools::saveRawDataSetAsVideo(RawDataSet* data, const QString& filename)
{

	av_register_all();

	/*
	{25, "CelB",       AVMEDIA_TYPE_VIDEO,   CODEC_ID_NONE, 90000, -1},
	00063   {26, "JPEG",       AVMEDIA_TYPE_VIDEO,   CODEC_ID_MJPEG, 90000, -1},
	00064   {28, "nv",         AVMEDIA_TYPE_VIDEO,   CODEC_ID_NONE, 90000, -1},
	00065   {31, "H261",       AVMEDIA_TYPE_VIDEO,   CODEC_ID_H261, 90000, -1},
	00066   {32, "MPV",        AVMEDIA_TYPE_VIDEO,   CODEC_ID_MPEG1VIDEO, 90000, -1},
	00067   {32, "MPV",        AVMEDIA_TYPE_VIDEO,   CODEC_ID_MPEG2VIDEO, 90000, -1},
	00068   {33, "MP2T",       AVMEDIA_TYPE_DATA,    CODEC_ID_MPEG2TS, 90000, -1},
	00069   {34, "H263",       AVMEDIA_TYPE_VIDEO,   CODEC_ID_H263, 90000, -1},
	*/
	AVDictionary** optionsDict = NULL;
	AVCodec* codec;
	AVCodecContext* c = NULL;
	int i, j, out_size, size, x, y, outbuf_size;
	FILE* f;
	AVFrame* picture;
	uint8_t* outbuf, * picture_buf;

	printf("Video encoding\n");

	/* find the mpeg1 video encoder */
	codec = avcodec_find_encoder(AV_CODEC_ID_MPEG4);
	if (!codec) {
		fprintf(stderr, "codec not found\n");
		exit(1);
	}

	c = avcodec_alloc_context3(codec);
	picture = av_frame_alloc();

	/* put sample parameters */
	c->bit_rate = 100000000;
	/* resolution must be a multiple of two */
	c->width = data->GetNx();
	c->height = data->GetNy();
	//  AVRational aspect;
	//  aspect.den=data->GetYscale()*1000;
	//aspect.num=data->GetXscale()*1000;

	//c->sample_aspect_ratio=aspect;
	//c->priv
	/* frames per second */
	AVRational ar;
	ar.den =50;
	ar.num = 1;
	c->time_base = ar;
	c->gop_size = 10; /* emit one intra frame every ten frames */
	c->max_b_frames = 1;
	c->pix_fmt = AV_PIX_FMT_YUV420P;
	// c->bit_rate_tolerance=10000;
	/* open it */

	//**********************
	//trick na voxelsizes?

	//c->lumi_masking=data->GetXscale();
	//c->temporal_cplx_masking=data->GetYscale();
	//c->spatial_cplx_masking=data->GetZscale();

	//**********************


	if (avcodec_open2(c, codec, optionsDict) < 0) {
		fprintf(stderr, "could not open codec\n");
		exit(1);
	}

	f = fopen(filename.toStdString().c_str(), "wb");
	if (!f) {
		fprintf(stderr, "could not open %s\n", filename);
		exit(1);
	}

	/* alloc image and output buffer */
	outbuf_size = 100000;
	outbuf = (uint8_t*)malloc(outbuf_size);
	size = c->width * c->height;
	picture_buf = (uint8_t*)malloc((size * 3) / 2); /* size for YUV 420 */

	picture->data[0] = picture_buf;
	picture->data[1] = picture->data[0] + size;
	picture->data[2] = picture->data[1] + size / 4;
	picture->linesize[0] = c->width;
	picture->linesize[1] = c->width / 2;
	picture->linesize[2] = c->width / 2;


	//bgra

	unsigned char* img_bgra = new unsigned char[data->GetNx() * data->GetNy() * 4];
	uint8_t* srcplanes[3];
	srcplanes[0] = (uint8_t*)img_bgra;
	srcplanes[1] = 0;
	srcplanes[2] = 0;
	int srcstride[3];
	srcstride[0] = data->GetNx() * 4;
	srcstride[1] = 0;
	srcstride[2] = 0;

	double scaler = 1.0;
	int sliceSize = data->GetSliceSize();
	if (data->GetMaxValue() > 255)
	{
		scaler = 255.0 / data->GetMaxValue();
	}
	unsigned short* tab = data->GetDataArray();
	unsigned char val;
	int pos;
	int r, g, b, a;
	int pos2;
	img_convert_ctx = NULL;
	for (i = 0; i < data->GetNz(); i++)
	{

		pos = i * sliceSize;
		fflush(stdout);

		//preapre img
		pos2 = 0;
		for (j = 0; j < data->GetSliceSize(); j++)
		{

			r = tab[pos + j] * scaler;
			g = r;
			b = r;
			a = 255;
			img_bgra[pos2++] = b;
			img_bgra[pos2++] = g;
			img_bgra[pos2++] = r;
			img_bgra[pos2++] = a;

		}



		img_convert_ctx = sws_getCachedContext(img_convert_ctx, data->GetNx(), data->GetNy(), AV_PIX_FMT_BGRA, data->GetNx(), data->GetNy(), AV_PIX_FMT_YUV420P, SWS_BICUBIC, NULL, NULL, NULL);

		sws_scale(img_convert_ctx, srcplanes, srcstride, 0, data->GetNy(), picture->data, picture->linesize);



		/* encode the image */
		//out_size = avcodec_encode_video2(c, outbuf, outbuf_size, picture);


		AVPacket pkt2;
		int got_packet = 0;
		av_init_packet(&pkt2);
		pkt2.data = outbuf;
		pkt2.size = outbuf_size;

		out_size = avcodec_encode_video2(c, &pkt2, picture, &got_packet);

		if (!out_size && got_packet && c->coded_frame)
		{
			c->coded_frame->pts = pkt2.pts;
			c->coded_frame->key_frame = !!(pkt2.flags & AV_PKT_FLAG_KEY);
			if (c->codec->capabilities & AV_CODEC_CAP_INTRA_ONLY)
				c->coded_frame->pict_type = AV_PICTURE_TYPE_I;
		}

		if (pkt2.side_data_elems > 0)
		{
			int i;
			for (i = 0; i < pkt2.side_data_elems; i++)
				av_free(pkt2.side_data[i].data);
			av_freep(&pkt2.side_data);
			pkt2.side_data_elems = 0;
		}

		if (!out_size)
			out_size = pkt2.size;






		printf("encoding frame %3d (size=%5d)\n", i, out_size);
		fwrite(outbuf, 1, out_size, f);
	}
	delete[]img_bgra;
	/* get the delayed frames */
	for (; out_size; i++) {
		fflush(stdout);

		//		out_size = avcodec_encode_video(c, outbuf, outbuf_size, NULL);


		AVPacket pkt2;
		int got_packet = 0;
		av_init_packet(&pkt2);
		pkt2.data = outbuf;
		pkt2.size = outbuf_size;

		out_size = avcodec_encode_video2(c, &pkt2, NULL, &got_packet);

		if (!out_size && got_packet && c->coded_frame)
		{
			c->coded_frame->pts = pkt2.pts;
			c->coded_frame->key_frame = !!(pkt2.flags & AV_PKT_FLAG_KEY);
			if (c->codec->capabilities & AV_CODEC_CAP_INTRA_ONLY)
				c->coded_frame->pict_type = AV_PICTURE_TYPE_I;
		}

		if (pkt2.side_data_elems > 0)
		{
			int i;
			for (i = 0; i < pkt2.side_data_elems; i++)
				av_free(pkt2.side_data[i].data);
			av_freep(&pkt2.side_data);
			pkt2.side_data_elems = 0;
		}

		if (!out_size)
			out_size = pkt2.size;


		printf("write frame %3d (size=%5d)\n", i, out_size);
		fwrite(outbuf, 1, out_size, f);
	}

	/* add sequence end code to have a real mpeg file */
	outbuf[0] = 0x00;
	outbuf[1] = 0x00;
	outbuf[2] = 0x01;
	outbuf[3] = 0xb7;
	fwrite(outbuf, 1, 4, f);
	fclose(f);
	free(picture_buf);
	free(outbuf);

	avcodec_close(c);
	av_free(c);
	av_free(picture);
	printf("\n");
}