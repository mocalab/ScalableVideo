#include "mediastatistics.h"

MediaStatistics::MediaStatistics()
{
}

float MediaStatistics::getDemuxBitrate() const
{
    return demuxBitrate;
}

void MediaStatistics::setDemuxBitrate(float value)
{
    demuxBitrate = value;
}

float MediaStatistics::getInputBitrate() const
{
    return inputBitrate;
}

void MediaStatistics::setInputBitrate(float value)
{
    inputBitrate = value;
}

int MediaStatistics::getLostPictures() const
{
    return lostPictures;
}

void MediaStatistics::setLostPictures(int value)
{
    lostPictures = value;
}

int MediaStatistics::getPlayedPictures() const
{
    return playedPictures;
}

void MediaStatistics::setPlayedPictures(int value)
{
    playedPictures = value;
}

int MediaStatistics::getDisplayedPictures() const
{
    return displayedPictures;
}

void MediaStatistics::setDisplayedPictures(int value)
{
    displayedPictures = value;
}

int MediaStatistics::getDecodedVideo() const
{
    return decodedVideo;
}

void MediaStatistics::setDecodedVideo(int value)
{
    decodedVideo = value;
}

int MediaStatistics::getDemuxDiscontinuity() const
{
    return demuxDiscontinuity;
}

void MediaStatistics::setDemuxDiscontinuity(int value)
{
    demuxDiscontinuity = value;
}

int MediaStatistics::getDemuxCorrupted() const
{
    return demuxCorrupted;
}

void MediaStatistics::setDemuxCorrupted(int value)
{
    demuxCorrupted = value;
}

int MediaStatistics::getDemuxReadBytes() const
{
    return demuxReadBytes;
}

void MediaStatistics::setDemuxReadBytes(int value)
{
    demuxReadBytes = value;
}

int MediaStatistics::getReadBytes() const
{
    return readBytes;
}

void MediaStatistics::setReadBytes(int value)
{
    readBytes = value;
}
