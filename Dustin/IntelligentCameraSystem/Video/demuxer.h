#ifndef DEMUXER_H
#define DEMUXER_H

#include <fcntl.h>
#include <unistd.h>

#ifndef INT64_C
#define INT64_C(c) (c ## LL)
#define UINT64_C(c) (c ## ULL)
#endif
extern "C" {
#include <libavutil/imgutils.h>
#include <libavutil/samplefmt.h>

#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
}
#include <QObject>
#include <QImage>
#include <QSharedPointer>

class Demuxer : public QObject
{
    Q_OBJECT
public:
    explicit Demuxer(QString fifo, QObject *parent = 0);



    QString fifo() const;
    void setFifo(const QString &fifo);

signals:
    void finished();
    void newImage(QImage *);
public slots:
    void demux();
private:
    QString                 m_fifo;
    int                     video_frame_count;
    //Functions

    //FFMpeg structs
    AVFormatContext         *fmt_ctx;
    AVCodecContext          *dec_ctx;
    AVFrame                 *frame;
    AVPacket                pkt;
    uint8_t                 *destination_data[4];
    int                     destination_linesize[4];
    int                     buffsize;

    //FOR RGB32 CONVERSION
    struct SwsContext       *img_convert_context;
    enum PixelFormat        dest_pix_fmt;
    int                     dest_buffsize;

    int open_codec_context(int *stream_index, enum AVMediaType type);
    int decode_frame(FILE *filed, int *got_frame, int cached);
    int toRGB(int height);
    void freeAllResources(FILE *file);

};

#endif // DEMUXER_H
