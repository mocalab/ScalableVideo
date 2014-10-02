#include "decisioninterface.h"
#include "Global/Log.h"
#include <QString>

DecisionInterface::DecisionInterface(IVideoWindowManager *manager) :
    m_window_interface(manager),
    m_resolutions(NULL)
{
    //Set up the optimal bitrate matrices
    //Values for FPS_15 are set higher to compensate for encoder behavior
    //Low complexity
    m_betas[0][FPS_15][QCIF] = 150000;
    m_betas[0][FPS_15][CIF] = 200000;
    m_betas[0][FPS_30][QCIF] = 100000;
    m_betas[0][FPS_30][CIF] = 400000;

    //Medium complexity
    m_betas[1][FPS_15][QCIF] = 250000;
    m_betas[1][FPS_15][CIF] = 400000;
    m_betas[1][FPS_30][QCIF] = 260000;
    m_betas[1][FPS_30][CIF] = 500000;

    //High complexity
    m_betas[2][FPS_15][QCIF] = 500000;
    m_betas[2][FPS_15][CIF] = 800000;
    m_betas[2][FPS_30][QCIF] = 500000;
    m_betas[2][FPS_30][CIF] = 850000;
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

//Determine what the new encoding parameters should be using the method with \beta
void DecisionInterface::makeDecisionUsingBeta(int bandwidth,
                                     int &datarate,
                                     EncodingParameters &old_params,
                                     int class_mask,
                                     EncodingParameters &new_params,
                                     int complexity)
{
    int width = 0, height = 0, max_bitrate = 0, optimum_bitrate = 0, bitrate = 0, i = 0, fps;
    float ceiling = OPTIMUM_CEILING;
    QStringList split;
    QString res;
    //Because we get the bw in kbps
    bandwidth *= 1000;
    max_bitrate = bandwidth * ceiling;
    //See what the user's class is
    switch(class_mask)
    {
        //Maximize all
        case 0:
            bitrate = max_bitrate < m_betas[complexity][FPS_30][CIF] ? max_bitrate : m_betas[complexity][FPS_30][CIF];
            res = m_possible_resolutions[0];
            split = res.split('x');
            width = split[0].toInt();//in.widthAsInt();
            height = split[1].toInt();//in.heightAsInt();
            fps = 30;
            break;
        //Frame rate > frame size
        case 1:
            fps = 30;
            if(m_betas[complexity][FPS_30][QCIF] > max_bitrate)
            {
                bitrate = max_bitrate;
                width = 176;
                height = 144;
            }
            else if(m_betas[complexity][FPS_30][CIF] > max_bitrate)
            {
                bitrate = m_betas[complexity][FPS_30][QCIF];
                width = 176;
                height = 144;
            }
            else
            {
                bitrate = max_bitrate;
                width = 352;
                height = 256;
            }
            break;
        //Frame size > frame rate
        case 2:
            res = m_possible_resolutions[0];
            split = res.split('x');
            width = split[0].toInt();//in.widthAsInt();
            height = split[1].toInt();//in.heightAsInt();
            if(m_betas[complexity][FPS_15][CIF] > max_bitrate)
            {
                bitrate = max_bitrate;
                fps = 15;
            }
            else if(m_betas[complexity][FPS_30][CIF] > max_bitrate)
            {
                bitrate = m_betas[complexity][FPS_15][CIF];
                fps = 15;
            }
            else
            {
                bitrate = max_bitrate;
                fps = 30;
            }
            break;
        //Quality most important
        case 3:
            bitrate = max_bitrate;
            //Very limited bandwidth, minimize everything
            if(m_betas[complexity][FPS_15][QCIF] > max_bitrate)
            {
                bitrate = max_bitrate;
                width = 176;
                height = 144;
                fps = 15;
            }
            else if(m_betas[complexity][FPS_30][QCIF] > max_bitrate)
            {
                bitrate = max_bitrate;
                width = 176;
                height = 144;
                fps = 15;
            }
            else if(m_betas[complexity][FPS_15][CIF] > max_bitrate)
            {
                bitrate = max_bitrate;
                width = 176;
                height = 144;
                fps = 30;
            }
            else if(m_betas[complexity][FPS_30][CIF] > max_bitrate)
            {
                bitrate = max_bitrate;
                width = 352;
                height = 288;
                fps = 15;
            }
            else
            {
                bitrate = max_bitrate;
                width = 352;
                height = 288;
                fps = 30;
            }
            break;
    }
    float fps_mult = (float)fps / 17.28;
    new_params.setBitrate(QString::number((int)(bitrate / fps_mult)));
    new_params.setFps(QString::number(fps));
    new_params.setHeight(QString::number(height));
    new_params.setWidth(QString::number(width));
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

//This is the default action (as in when in learning mode)
void DecisionInterface::defaultAdjustBitrate(int bandwidth,
                                             int &datarate,
                                             EncodingParameters &in,
                                             EncodingParameters &out,
                                             int complexity)
{
    float ceiling = OPTIMUM_CEILING;
    //Determined empirically by looking at graphs of the actual video bitrate
    float fps_mult = (float)30 / 17.28;

    int max_bitrate = (int) bandwidth * ceiling * 1000 / fps_mult;

    QString res = m_possible_resolutions[0];
    QStringList split = res.split('x');
    int width = split[0].toInt();//in.widthAsInt();
    int height = split[1].toInt();//in.heightAsInt();
    out.setWidth(QString::number(width));
    out.setHeight(QString::number(height));

    out.setFps("30");

    //Decide the optimum bitrate that can be used
    int optimum_bitrate = m_betas[complexity][FPS_30][CIF];

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

int DecisionInterface::getOptimalBitrate(int max_bitrate, int fps, int width, int height, int complexity)
{
    int fps_idx = fps == 30 ? 1 : 0;

    int spatial_idx = width == 352 ? 1 : 0;

    //Decide the optimum bitrate that can be used
    int optimum_bitrate = m_betas[complexity][fps_idx][spatial_idx];
    //optimum_bitrate /= (fps / 17.28);
    return max_bitrate < optimum_bitrate ? (int) max_bitrate : (int) optimum_bitrate;
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
        res = m_possible_resolutions[0];
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
        res = m_possible_resolutions[0];
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
