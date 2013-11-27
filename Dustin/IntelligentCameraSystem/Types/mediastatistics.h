#ifndef MEDIASTATISTICS_H
#define MEDIASTATISTICS_H

/**
 * @brief A wrapper to VLC libvlc_media_stats_t struct.
 *This class serves as a C++ wrapper to the libvlc_media_stats_t struct defined by VLC. It
 *encapsulates only the fields we may be interested in.
 */
class MediaStatistics
{
public:
    /**
     * @brief MediaStatistics constructor.
     */
    MediaStatistics();

    int getReadBytes() const;
    void setReadBytes(int value);

    int getDemuxReadBytes() const;
    void setDemuxReadBytes(int value);

    int getDemuxCorrupted() const;
    void setDemuxCorrupted(int value);

    int getDemuxDiscontinuity() const;
    void setDemuxDiscontinuity(int value);

    int getDecodedVideo() const;
    void setDecodedVideo(int value);

    int getDisplayedPictures() const;
    void setDisplayedPictures(int value);

    int getPlayedPictures() const;
    void setPlayedPictures(int value);

    int getLostPictures() const;
    void setLostPictures(int value);

    float getInputBitrate() const;
    void setInputBitrate(float value);

    float getDemuxBitrate() const;
    void setDemuxBitrate(float value);

private:
    //Integer members
    int                 readBytes;
    int                 demuxReadBytes;
    int                 demuxCorrupted;
    int                 demuxDiscontinuity;
    int                 decodedVideo;
    int                 displayedPictures;
    int                 playedPictures;
    int                 lostPictures;

    //Float members
    float               inputBitrate;
    float               demuxBitrate;
};

#endif // MEDIASTATISTICS_H
