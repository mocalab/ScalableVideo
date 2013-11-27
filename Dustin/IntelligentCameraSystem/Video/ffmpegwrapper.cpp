#include <cstdio>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <netdb.h>
#include <errno.h>

#include <Global/Log.h>

#include "ffmpegwrapper.h"

FFMPEGWrapper::FFMPEGWrapper(int port, Demuxer *demuxer, QObject *parent) :
    QObject(parent),
    port(port)
{
    if(demuxer == NULL)
        m_demuxer = new Demuxer(QString(PIPENAME));
    else
        m_demuxer = demuxer;

    //Set up the demuxer thread
    m_demux_thread = new QThread(this);
    m_demuxer->moveToThread(m_demux_thread);
    connect(m_demux_thread, SIGNAL(started()), m_demuxer, SLOT(demux()));
    connect(m_demuxer, SIGNAL(finished()), m_demux_thread, SLOT(quit()));

}

void FFMPEGWrapper::demux()
{
    int serv_sockfd, num_bytes;

    char packet[PKT_SIZE];

    //The file
    FILE *buffer;

    //Storage for sender's address
    struct sockaddr_storage their_addr;
    socklen_t addr_len;

    //Start a UDP server -- open up a port
    //Open a socket
    serv_sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if(serv_sockfd <= 0)
    {
        L_ERROR() << "Failed to open socket.";
        //perror("socket");
        return;
    }

    //Bind to port on host side
    struct sockaddr_in address;

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons((unsigned short) port);

    int bound = bind(serv_sockfd, (struct sockaddr *) &address, sizeof(address));
    if(bound < 0)
    {
        L_ERROR() << "Failed to bind socket to port";
        //perror("bind");
        return;
    }

    //Create a named pipe (FIFO)

    char *fifo = PIPENAME;
    mkfifo(fifo, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH);

    //=====CHANGE CODE HERE TO START A QT THREAD FOR DEMUXER OBJ TO PERFORM ITS WORK
    m_demux_thread->start();

    //Open pipe
    //Open the file
    if((buffer = fopen(PIPENAME, "wb")) == NULL)
    {
        //perror("fopen");
        //exit(1);
    }

    is_demuxing = true;

    INFO() << "Start demuxing";

    while(is_demuxing)
    {

        memset(packet, 0, PKT_SIZE);

        //Get next packet
        if((num_bytes = recvfrom(serv_sockfd, packet, PKT_SIZE, 0,
            (struct sockaddr *)&their_addr, &addr_len)) == -1)
        {
            if(errno != EAGAIN)
            {
                L_ERROR() << "recvfrom: " << strerror(errno);
                is_demuxing = false;
            }
        }

        if(errno != EAGAIN)
        {
            char *dat = packet;
            //if(packet[4] != 0x67 && packet[4] != 0x65)
            //{
                //dat++;
                //num_bytes--;
            //}
            //Write to file
            fwrite(dat, sizeof(char), (num_bytes) / sizeof(char), buffer);
        }

    }

    INFO() << "End demuxing";
    close(serv_sockfd);
    fclose(buffer);

    emit finished();

    return;

}

void FFMPEGWrapper::gotImage(QSharedPointer<QImage> img)
{
    emit newImage(img);
}


bool FFMPEGWrapper::isDemuxing() const
{
    return is_demuxing;
}

void FFMPEGWrapper::stop()
{
    is_demuxing = false;
}
