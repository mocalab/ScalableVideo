#include "decisioninterface.h"
#include "Global/Log.h"
#include <QString>

DecisionInterface::DecisionInterface(IVideoWindowManager *manager) :
    m_window_interface(manager),
    m_resolutions(NULL)
{

}

DecisionInterface::~DecisionInterface()
{
    if(m_resolutions != NULL)
        delete m_resolutions;
}

//Determine what the new encoding parameters should be
void DecisionInterface::makeDecision(int bandwidth,
                                     int &datarate,
                                     EncodingParameters &old_params,
                                     int class_mask,
                                     EncodingParameters &new_params)
{
    //Get the ratio of bandwidth to data rate;
    float ratio = (float) datarate / bandwidth;
    DEBUG() << "Ratio: " << ratio;
    //Functionality depends on the user's preferences
    if(ratio < 1)
    {
        upConvert(ratio, old_params, new_params, class_mask, datarate);
    }
    else
    {
        downConvert(ratio, old_params, new_params, class_mask, datarate);
    }

    //This determines the next course of action
//    if(ratio < 1)
//    {
//        //We can optimize the parameters based on the available bandwidth
//        upConvert(ratio, old_params, new_params);
//    }
//    else
//    {
//        //We must determine the best parameters using the machine learning decision function we have
//        //created while the application was learning

//        //For now, just keep the old parameters
//        downConvert(ratio, old_params, new_params);
//    }

    return;
}

//This is the default action (as in when in learning mode)
void DecisionInterface::defaultAdjustBitrate(int bandwidth,
                                             int &datarate,
                                             EncodingParameters &in,
                                             EncodingParameters &out)
{
    float ceiling = OPTIMUM_CEILING;
    float ratio = (float)datarate / bandwidth;

    int max_bitrate = (int)(((float)in.bitrateAsInt() / ratio) * ceiling);

    int width = in.widthAsInt();
    int height = in.heightAsInt();
    out.setWidth(QString::number(width));
    out.setHeight(QString::number(height));
    //Decide the optimum bitrate that can be used
    int optimum_bitrate = width * height * 3.5;

    int bitrate = max_bitrate < optimum_bitrate ? max_bitrate : optimum_bitrate;

    out.setBitrate(QString::number(bitrate));

    //Set the new bitrate for the caller
    //datarate /= ratio;
}

void DecisionInterface::setResolutionsList(QStringList &resolutions)
{
    if(m_resolutions != NULL)
        delete m_resolutions;
    int list_size = resolutions.count();
    this->m_resolutions = new int[list_size];
    for(int i = 0; i < list_size; i++)
    {
        QString next = resolutions[i];
        QStringList width_by_height = next.split('x');

        int width = width_by_height[0].toInt();
        int height = width_by_height[1].toInt();

        m_resolutions[i] = width * height;
    }

    m_possible_resolutions = resolutions;
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
    int max_bitrate = (int)(((float)in.bitrateAsInt() / ratio) * ceiling);

    //Choose the least of these two bitrates
    int bitrate = max_bitrate < optimum_bitrate ? max_bitrate : optimum_bitrate;
    out.setBitrate(QString::number(bitrate));
}

//Optimize parameters in the order specified by the class mask
void DecisionInterface::upConvert(float ratio, EncodingParameters &in, EncodingParameters &out, int class_mask, int& datarate)
{
    int width = 0, height = 0, max_bitrate = 0, optimum_bitrate = 0, bitrate = 0, i = 0;
    float ceiling = OPTIMUM_CEILING;
    QStringList split;
    QString res;
    //Use class mask to determine order
    switch(class_mask)
    {
    //User prefers frame rate and size
    case 0:
        //Increase frame rate if possible

        if(in.fpsAsInt() < 30 /*&& (2*ratio) < ceiling*/)
        {
            //Optimize the framerate
            out.setFps(QString::number(30));
            ratio *= 2;
        }

        //Increase bitrate if possible
        //Decide the optimum bitrate that can be used
        res = m_possible_resolutions[m_possible_resolutions.count() - 1];
        split = res.split('x');
        width = split[0].toInt();//in.widthAsInt();
        height = split[1].toInt();//in.heightAsInt();
        optimum_bitrate = width * height * 3.5;

        //Determine the max bitrate available
        max_bitrate = (int)(((float)in.bitrateAsInt() / ratio) * ceiling);

        //Choose the least of these two bitrates
        bitrate = max_bitrate < optimum_bitrate ? max_bitrate : optimum_bitrate;
        out.setBitrate(QString::number(bitrate));

        //Increase size to max user desired
        //WILL HAVE TO INTELLIGENTLY DECIDE THIS

        out.setWidth(QString::number(width));
        out.setHeight(QString::number(height));

        break;
    //User prefers frame rate and quality
    case 1:
        //Increase frame rate if possible
        ceiling = OPTIMUM_CEILING;
        if(in.fpsAsInt() < 30 /*&& (2*ratio) < ceiling*/)
        {
            //Optimize the framerate
            out.setFps(QString::number(30));
            ratio *= 2;
        }
        //Increase bitrate if possible
        //Decide the optimum bitrate that can be used
//        width = in.widthAsInt();
//        height = in.heightAsInt();
//        optimum_bitrate = width * height * 3.5;

        //Determine the max bitrate available
        max_bitrate = (int)(((float)in.bitrateAsInt() / ratio) * ceiling);

        //Choose the least of these two bitrates
        bitrate = max_bitrate;// < optimum_bitrate ? max_bitrate : optimum_bitrate;
        out.setBitrate(QString::number(bitrate));

        //Change size to smallest necessary to provide best quality
        for(i = 0; i < m_possible_resolutions.count(); i++)
        {
            if(bitrate > m_resolutions[i] * 3.5)
                break;
        }
        if(i == m_possible_resolutions.count())
            i--;
        res = m_possible_resolutions[i];
        split = res.split('x');
        out.setWidth(split[0]);
        out.setHeight(split[1]);
        break;
    //User prefers bitrate and size
    case 2:
        //Increase bitrate
        res = m_possible_resolutions[m_possible_resolutions.count() - 1];
        split = res.split('x');
        width = split[0].toInt();//in.widthAsInt();
        height = split[1].toInt();//in.heightAsInt();
        optimum_bitrate = width * height * 3.5;

        //Determine the max bitrate available
        max_bitrate = (int)(((float)in.bitrateAsInt() / ratio) * ceiling);

        //Choose the least of these two bitrates
        bitrate = max_bitrate < optimum_bitrate ? max_bitrate : optimum_bitrate;
        out.setBitrate(QString::number(bitrate));
        //Increase size to max user desired
        //WILL HAVE TO INTELLIGENTLY DECIDE THIS

        out.setWidth(QString::number(width));
        out.setHeight(QString::number(height));

        //See if we should increase frame rate (if possible)
        if(((float)bitrate / in.bitrateAsInt()) * ratio < (0.5 * ceiling) && in.fpsAsInt() < 30)
        {
            out.setFps(QString::number(30));
        }
        break;
    //User prefers bitrate and quality
    case 3:
        //Increase bitrate
        //Decide the optimum bitrate that can be used
//        width = in.widthAsInt();
//        height = in.heightAsInt();
//        optimum_bitrate = width * height * 3.5;

        //Determine the max bitrate available
        max_bitrate = (int)(((float)in.bitrateAsInt() / ratio) * ceiling);

        //Choose the least of these two bitrates
        bitrate = max_bitrate;// < optimum_bitrate ? max_bitrate : optimum_bitrate;
        out.setBitrate(QString::number(bitrate));


        //Change size to smallest necessary to provide best quality
        for(i = 0; i < m_possible_resolutions.count(); i++)
        {
            if(bitrate > m_resolutions[i] * 3.5)
                break;
        }
        if(i == m_possible_resolutions.count())
            i--;
        res = m_possible_resolutions[i];
        split = res.split('x');
        out.setWidth(split[0]);
        out.setHeight(split[1]);

        break;
    }

    //Set the new datarate
    //datarate = (int)(datarate * ((float)out.bitrateAsInt() / (float)in.bitrateAsInt()) * ((float)out.fpsAsInt() / (float)in.fpsAsInt()));
}

//Down convert the encoding parameters
void DecisionInterface::downConvert(float ratio, EncodingParameters &in, EncodingParameters &out)
{
    //For now, just lower the bitrate to fit the channel
    float ceiling = OPTIMUM_CEILING;

    int max_bitrate = (int)(((float)in.bitrateAsInt() / ratio) * ceiling);

    out.setBitrate(QString::number(max_bitrate));
}

//Minimize parameters in the order specified by the class mask
void DecisionInterface::downConvert(float ratio, EncodingParameters &in, EncodingParameters &out, int class_mask, int &datarate)
{
    int bitrate = 0, width = 0, height = 0, max_bitrate = 0, optimum_bitrate = 0, i = 0;
    float ceiling = OPTIMUM_CEILING;
    QStringList split;
    QString res;
    //Use class mask to determine order
    switch(class_mask)
    {
    //User prefers frame rate and size
    case 0:
        //First, see if we should increase the frame rate
        if(in.fpsAsInt() < 30 /*&& (2*ratio) < ceiling*/)
        {
            //Optimize the framerate
            out.setFps(QString::number(30));
            ratio *= 2;
        }
        //Decrease bitrate to fit channel
        bitrate = (int)(((float)in.bitrateAsInt() / ratio) * ceiling);

        out.setBitrate(QString::number(max_bitrate));
        //Keep size constant
        out.setWidth(in.width());
        out.setHeight(in.height());
        break;

    //User prefers frame rate and quality
    case 1:
        if(in.fpsAsInt() < 30 /*&& (2*ratio) < ceiling*/)
        {
            //Optimize the framerate
            out.setFps(QString::number(30));
            ratio *= 2;
        }
        //Decrease bitrate to fit channel
        ceiling = OPTIMUM_CEILING;

        bitrate = (int)(((float)in.bitrateAsInt() / ratio) * ceiling);

        out.setBitrate(QString::number(bitrate));
        //Decrease size to increase quality

        //Change size to smallest necessary to provide best quality
        for(i = 0; i < m_possible_resolutions.count(); i++)
        {
            if(bitrate > m_resolutions[i] * 3.5)
                break;
        }
        if(i == m_possible_resolutions.count())
            i--;
        res = m_possible_resolutions[i];
        split = res.split('x');
        out.setWidth(split[0]);
        out.setHeight(split[1]);
        break;

    //User prefers bitrate and size
    case 2:
        //Decrease frame rate if possible
        if(in.fpsAsInt() > 15)
        {
            out.setFps(QString::number(15));
            ratio /= 2;
        }
        //Fit bitrate to channel
        width = in.widthAsInt();
        height = in.heightAsInt();
        optimum_bitrate = width * height * 3.5;

        //Determine the max bitrate available
        max_bitrate = (int)(((float)in.bitrateAsInt() / ratio) * ceiling);

        //Choose the least of these two bitrates
        bitrate = max_bitrate;// < optimum_bitrate ? max_bitrate : optimum_bitrate;
        out.setBitrate(QString::number(bitrate));

        //Keep size constant?

        break;
    //User prefers bitrate and quality
    case 3:
        //Decrese frame rate if possible

        if(in.fpsAsInt() > 15)
        {
            out.setFps(QString::number(15));
            ratio /= 2;
        }
        //Fit bitrate to channel
//        width = in.widthAsInt();
//        height = in.heightAsInt();
//        optimum_bitrate = width * height * 3.5;

        //Determine the max bitrate available
        max_bitrate = (int)(((float)in.bitrateAsInt() / ratio) * ceiling);

        //Choose the least of these two bitrates
        bitrate = max_bitrate;// < optimum_bitrate ? max_bitrate : optimum_bitrate;
        out.setBitrate(QString::number(bitrate));


        //Decrease size to increase quality
        //Change size to smallest necessary to provide best quality
        for(i = 0; i < m_possible_resolutions.count(); i++)
        {
            if(bitrate > m_resolutions[i] * 3.5)
                break;
        }
        if(i == m_possible_resolutions.count())
            i--;
        res = m_possible_resolutions[i];
        split = res.split('x');
        out.setWidth(split[0]);
        out.setHeight(split[1]);
        break;
    }
    //Set the new datarate
    //datarate = (int)(datarate * ((float)out.bitrateAsInt() / (float)in.bitrateAsInt()) * ((float)out.fpsAsInt() / (float)in.fpsAsInt()));
}
