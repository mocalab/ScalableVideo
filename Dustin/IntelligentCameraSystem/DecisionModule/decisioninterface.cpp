#include "decisioninterface.h"

DecisionInterface::DecisionInterface(IVideoWindowManager *manager) :
    m_window_interface(manager)
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
    //Determine what the video size should be

    //WILL HAVE TO INTELLIGENTLY DECIDE THIS
    int width = in.widthAsInt();
    int height = in.heightAsInt();
    out.setWidth(QString::number(width));
    out.setHeight(QString::number(height));
    //Decide the optimum bitrate that can be used
    int optimum_bitrate = width * height * 3.5;

    //Determine the max bitrate available
    int max_bitrate = (int)((in.bitrateAsInt() / ratio) * ceiling);

    //Choose the least of these two bitrates
    int bitrate = max_bitrate < optimum_bitrate ? max_bitrate : optimum_bitrate;
    out.setBitrate(QString::number(bitrate));
}

//Down convert the encoding parameters
void DecisionInterface::downConvert(float ratio, EncodingParameters &in, EncodingParameters &out)
{

}
