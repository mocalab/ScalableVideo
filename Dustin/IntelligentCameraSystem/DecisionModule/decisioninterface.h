/**
 *  @file Definiton of the DecisionInterface class.
 */
#ifndef DECISIONINTERFACE_H
#define DECISIONINTERFACE_H

///Keep any optimum at 95% of the average bandwidth in order to be resistant against fluctuations. This macro should be toggled until
///and acceptable value is determined
#define OPTIMUM_CEILING         0.85
#define FPS_15                  0
#define QCIF                    0
#define FPS_30                  1
#define CIF                     1
#include <QStringList>
#include "Types/encodingparameters.h"
#include "Types/featureset.h"
#include "UI/ivideowindowmanager.h"

/**
 * @brief This class defines the interface for making a decision about the correct encoding parameters to use for a stream of video.
 */
class DecisionInterface
{
public:
    /**
     * @brief Default value constructor.
     */
    DecisionInterface(IVideoWindowManager *manager = 0);

    ~DecisionInterface();

    /**
     * @brief Make a decision about the encoding parameters.
     *This function will determine what the new encoding parameters should be based on the available bandwidth, the current video
     *bitrate, the current encoding parameters, and a set of features related to the video.
     * @param bandwidth The current channel bandwidth.
     * @param datarate The current average data rate of the video.
     * @param old_params The old encoding parameters.
     * @param class_mask The class that the user falls in, predicted by the support vector machines.
     * @param new_params Object to hold the new encoding parameters.
     */
     void makeDecision(int bandwidth,
                       int &datarate,
                       EncodingParameters &old_params,
                       int class_mask,
                       EncodingParameters &new_params);

     void makeDecisionUsingBeta(int bandwidth,
                                          int &datarate,
                                          EncodingParameters &old_params,
                                          int class_mask,
                                          EncodingParameters &new_params,
                                          int complexity);

     /**
      * @brief By default, adjust the bitrate to fit the channel.
      * @param bandwidth The video bandwidth.
      * @param datarate The old data rate of the video.
      * @param in The old encoding parameters.
      * @param out The new encoding parameters.
      */
     void defaultAdjustBitrate(int bandwidth,
                               int &datarate,
                               EncodingParameters &in,
                               EncodingParameters &out);

     void defaultAdjustBitrate(int bandwidth,
                                  int &datarate,
                                  EncodingParameters &in,
                                  EncodingParameters &out,
                                  int complexity);

     /**
      * @brief Set the internal resolutions list.
      * @param resolutions The resolutions available.
      */
     void setResolutionsList(QStringList &resolutions);

     int getOptimalBitrate(int max_bitrate, int fps, int width, int height, int complexity);

private:
    /**
     * @brief Up convert the encoding parameters.
     *If the available bandwidth is greater than the data rate, we can optimize the encoding parameters by increasing desired
     *parameters.
     * @param ratio The ratio of datarate / bandwidth.
     * @param in The pre-converted parameters.
     * @param out An object to hold the up converted parameters.
     */
    void upConvert(float ratio, EncodingParameters &in, EncodingParameters &out);

    /**
     * @brief Up convert the encoding parameters.
     *If the available bandwidth is greater than the data rate, we can optimize the encoding parameters by increasing desired
     *parameters.
     * @param ratio The ratio of datarate / bandwidth.
     * @param in The pre-converted parameters.
     * @param out An object to hold the up converted parameters.
     * @param class_mask Determines the order in which we optimize parameters.
     * @param datarate Reference to the old datarate which needs to be updated.
     */
    void upConvert(float ratio, EncodingParameters &in, EncodingParameters &out, int class_mask, int &datarate);

    /**
     * @brief Down convert the encoding parameters.
     *If the available bandwidth is less than the data rate, we can optimize the encoding parameters by decreasing desired
     *parameters.
     * @param ratio The ratio of datarate / bandwidth.
     * @param in The pre-converted parameters.
     * @param out An object to hold the up converted parameters.
     */
    void downConvert(float ratio, EncodingParameters &in, EncodingParameters &out);

    /**
     * @brief Down convert the encoding parameters.
     *If the available bandwidth is less than the data rate, we can optimize the encoding parameters by decreasing desired
     *parameters.
     * @param ratio The ratio of datarate / bandwidth.
     * @param in The pre-converted parameters.
     * @param out An object to hold the up converted parameters.
     * @param class_mask Determines the order in which we minimize parameters.
     * @param datarate Reference to the old datarate which needs to be updated.
     */
    void downConvert(float ratio, EncodingParameters &in, EncodingParameters &out, int class_mask, int &datarate);

    //The video window manager
    IVideoWindowManager                         *m_window_interface;

    //A list of resolution widths * heights for the purpose of determining video sizes
    int                                         *m_resolutions;

    QStringList                                 m_possible_resolutions;

    //The matrices \beta. Each have 4 values as there are 2 possible frame rates and 2 possible frame sizes
    int                                         m_betas[3][2][2];
};

#endif // DECISIONINTERFACE_H
