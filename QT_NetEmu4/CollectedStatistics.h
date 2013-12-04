#ifndef COLLECTEDSTATISTICS_H
#define COLLECTEDSTATISTICS_H

struct CollectedStatistics
{
    int packetsSent1;
    int packetsSent2;
    int packetsDropped1;
    int packetsDropped2;
    float bandwidth1;
    float bandwidth2;
    int bufferSize1;
    int bufferSize2;
};

#endif // COLLECTEDSTATISTICS_H
