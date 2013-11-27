#include "demuxer.h"
#include <Global/Log.h>

Demuxer::Demuxer(QString fifo, QObject *parent) :
    QObject(parent),
    m_fifo(fifo),
    video_frame_count(0),
    destination_data({NULL})
{
}



void Demuxer::demux()
{
    //Some flags
    int ret = 0, got_frame;
    FILE *outfile_fd = 0;

    //int to hold stream index
    int video_stream_index = -1;

    //AVFormatContext
    fmt_ctx = NULL;

    //Video Stream
    AVStream *video_stream = NULL;

    //Decoder context
    dec_ctx = NULL;

    //Frame pointer and packet
    frame = NULL;

    //Register formats and codecs
    av_register_all();

    if(avformat_open_input(&fmt_ctx, m_fifo.toStdString().c_str(), NULL, NULL) < 0)
    {
        L_ERROR() << "Unable to open infile\n";
        return;
    }

    //Find stream information
    if(avformat_find_stream_info(fmt_ctx, NULL) < 0)
    {
        L_ERROR() << "Unable to find stream info\n";
        return;
    }

    //Try to open the decoder
    if(open_codec_context(&video_stream_index, AVMEDIA_TYPE_VIDEO) >= 0)
    {
        //Get the stream
        video_stream = fmt_ctx->streams[video_stream_index];
        //Get the decoder context
        dec_ctx = video_stream->codec;

        //Open the destination file NEED TO CHANGE TO SEND DECODED PACKETS TO WINDOW
        if((outfile_fd = fopen("out.raw", "wb")) == NULL)
        {

            freeAllResources(outfile_fd);
            return;
        }

        //Determine the size needed for each frame, allocate buffers
        ret = av_image_alloc(destination_data, destination_linesize,
                                    dec_ctx->width, dec_ctx->height,
                                    dec_ctx->pix_fmt, 1);

        if(ret < 0)
        {
            L_ERROR() << "Unable to allocate image buffer\n";
            freeAllResources(outfile_fd);
            return;
        }

        buffsize = ret;

    }

    //Set up the image convert context
    dest_pix_fmt = PIX_FMT_RGB32;
    int w = dec_ctx->width;
    int h = dec_ctx->height;
    img_convert_context = sws_getContext(w, h,
                        dec_ctx->pix_fmt,
                       w, h, dest_pix_fmt, SWS_BICUBIC,
                       NULL, NULL, NULL);
   if(img_convert_context == NULL)
   {
        L_ERROR() << "Unable to initialize converter.\n";
        freeAllResources(outfile_fd);
        return;
   }
   dest_buffsize = avpicture_get_size(dest_pix_fmt, w, h);

    //Dump the format info to stderr
    //av_dump_format(format_context, 0, infile, 0);

    if(!video_stream)
    {
        L_ERROR() << "Failed to open anything! Goodbye!\n";
        return;
    }

    //Lets get a frame... We're done initializing I think
    frame = avcodec_alloc_frame();
    if(!frame)
    {
        L_ERROR() << "We couldn't get a frame! :(\n";
        ret = AVERROR(ENOMEM);
        freeAllResources(outfile_fd);
        return;
    }

    //Initalize a packet, set the data to null, let the demuxer fill that foo
    av_init_packet(&pkt);
    pkt.data = NULL;
    pkt.size = 0;

    if(video_stream)
        //INFO() << "Demuxing video from file %s into %s\n", infile, outfile;

    //The loop of decoding-ness
    while(av_read_frame(fmt_ctx, &pkt) >= 0)
    {
        AVPacket orig = pkt;
        do
        {
            ret = decode_frame(outfile_fd, &got_frame, 0);

            if(ret < 0)
                break;

            pkt.data += ret;
            pkt.size -= ret;
        }while(pkt.size > 0);

        //Free this packet
        av_free_packet(&orig);
    }

    //Flush the cached frames
    pkt.data = NULL;
    pkt.size = 0;
    do{
        decode_frame(outfile_fd, &got_frame, 1);
    }while(got_frame);

    INFO() << "Demuxing succeeded.\n";

    if (video_stream) {
            INFO() << "Play the output video file with the command:\n" <<
                      "ffplay -f rawvideo -pix_fmt " << av_get_pix_fmt_name(dest_pix_fmt) <<
                      "video_size " << dec_ctx->width <<"x" << dec_ctx->height << "out.raw\n";
        }

    //freeAllResources(format_context, dec_context, outfile_fd, frame, destination_data);

    //Emit finished signal
    emit finished();
    return;
}

int Demuxer::open_codec_context(int *stream_index, enum AVMediaType type)
{
    int ret;
    AVStream *stream;
    AVCodecContext *dec_context = NULL;
    AVCodec *dec = NULL;

    ret = av_find_best_stream(fmt_ctx, type, -1, -1, NULL, 0);

    if(ret < 0)
    {
        L_ERROR() <<  "Could not find stream!\n";
        return ret;
    }
    else
    {
        //return value is stream index
        *stream_index = ret;
        stream = fmt_ctx->streams[*stream_index];

        //Find the decoder!!!
        dec_context = stream->codec;
        dec = avcodec_find_decoder(dec_context->codec_id);

        if(!dec)
        {
            L_ERROR() << "Unable to find decoder\n";
            return ret;
        }

        //Open the decoder
        if((ret = avcodec_open2(dec_context, dec, NULL)) < 0)
        {
            L_ERROR() << "Failed to open decoder\n";
            return ret;
        }
    }

    return 0;
}

//Function to decode a packet
int Demuxer::decode_frame(FILE *filed, int *got_frame, int cached)
{
    int ret = 0;
    int decoded = pkt.size;

    //Decode the frame
    ret = avcodec_decode_video2(dec_ctx, frame, got_frame, &pkt);

    if(ret < 0)
    {
        L_ERROR() << "Unable to decode the frame!\n";
        return ret;
    }

    if(*got_frame)
    {
        //printf("video_frame%s n:%d coded_n:%d pts:%s\n",
                    //cached ? "(cached)" : "",
                    //video_frame_count++, frame->coded_picture_number,
                    //av_ts2timestr(frame->pts, &dec_ctx->time_base));

        //Copy to destination buffer
        av_image_copy(destination_data, destination_linesize,
                            (const uint8_t **)(frame->data), frame->linesize,
                            dec_ctx->pix_fmt, dec_ctx->width, dec_ctx->height);

        //Convert format to RGB32
        toRGB(dec_ctx->height);

        //Copy the data into a new mem location
        uchar *image_buff = new uchar[dest_buffsize];
        memcpy(image_buff, destination_data[0], dest_buffsize);

        //Pass this to the window as a new QImage
        QImage *img = new QImage(image_buff, dec_ctx->width, dec_ctx->height, QImage::Format_RGB32);
        //QSharedPointer<QImage> ptr(img);
        emit newImage(img);

        //Write this data to the file -- CHANGE THIS TO PASS TO VIDEO WINDOW
        //fwrite(destination_data[0], 1, buffsize, filed);
    }



    return decoded;

}

//Convert YUV420 frames to RGB32 frames
int Demuxer::toRGB(int height)
{
    int ret = 0;

    //Try to convert
    sws_scale(img_convert_context, (const uint8_t **)frame->data,
              frame->linesize, 0,
              height,
              destination_data, destination_linesize);
    return ret;
}

void Demuxer::freeAllResources(FILE *file)
{
    //Free our resources
    avcodec_close(dec_ctx);

    av_free(&fmt_ctx);

    //fclose(file);

    av_free(frame);
    av_free(destination_data);
}

QString Demuxer::fifo() const
{
    return m_fifo;
}

void Demuxer::setFifo(const QString &fifo)
{
    m_fifo = fifo;
}
