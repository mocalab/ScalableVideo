#include "decisioninterface.h"

DecisionInterface::DecisionInterface()
{
}

//Determine what the new encoding parameters should be
void DecisionInterface::makeDecision(int bandwidth, int datarate, EncodingParameters &old_params, FeatureSet &features, EncodingParameters &new_params)
{
    //Get the ratio of bandwidth to data rate;
    float ratio = (float) datarate / bandwidth;

    //This determines the next course of action
    if(ratio < 1)
    {
        //We can optimize the parameters based on the available bandwidth
        upConvert(ratio, old_params, new_params);
    }
    else
    {
        //We must determine the best parameters using the machine learning decision function we have
        //created while the application was learning

        //For now, just keep the old parameters
        new_params = old_params;
    }

    return;
}

//Up convert the encoding parameters
void DecisionInterface::upConvert(float ratio, EncodingParameters &in, EncodingParameters &out)
{
    float ceiling = OPTIMUM_CEILING;
    //First, see if we should increase the frame rate
    if(in.fpsAsInt() < 30 && (2*ratio) < ceiling)
    {
        //Optimize the framerate
        out.setFps(QString::number(30));
        ratio *= 2;
    }
    int bitrate = (int)((in.bitrateAsInt() / ratio) * ceiling);
    out.setBitrate(QString::number(bitrate));
}
