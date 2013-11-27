#ifndef FFMPEGWRAPPER_H
#define FFMPEGWRAPPER_H
#include <QObject>
#include <QThread>
#include <QImage>
#include <QSharedPointer>
#include "demuxer.h"
#define PKT_SIZE 65536
#define PIPENAME "/tmp/buffPipe1"


class FFMPEGWrapper : public QObject
{
    Q_OBJECT
public:
    FFMPEGWrapper(int port, Demuxer *demuxer = NULL, QObject *parent = 0);
    //Main function to demux a video stream over the provided port


    bool isDemuxing() const;
    void stop();

signals:
    void finished();
    void newImage(QSharedPointer<QImage>);
public slots:
    void demux();
    void gotImage(QSharedPointer<QImage> img);
private:
    Demuxer         *m_demuxer;
    QThread         *m_demux_thread;
    bool            is_demuxing;
    int             port;
};

#endif // FFMPEGWRAPPER_H
