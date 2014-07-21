#ifndef ICONTROLCENTERMANAGER_H
#define ICONTROLCENTERMANAGER_H

//Forward class declarations
class FeatureSet;

/**
 * @brief This class defines an interface to the control center window.
 *Any external modules that wish to obtain data from the control center window should access them through this interface.
 *Therefore, any data that should be made available to external modules should be accessible via this interface.
 */
class IControlCenterManager
{
public:

    /**
     * @brief Determine if the application is in learning mode or not.
     * @return True if the application is in learning mode, false if not.
     */
    virtual bool inLearningMode() = 0;

    /**
     * @brief Add a training example to the training set of the learning module managed by this
     *control center window.
     * @param fs The feature set.
     * @param lbl_fps_br_priority The label for the FPS vs. Bitrate trainer.
     * @param lbl_size_quality_priority The label for the size vs. quality trainer.
     */
    virtual void addTrainingExample(FeatureSet &fs, double lbl_fps_br_priority, double lbl_size_quality_priority) = 0;

    /**
     * @brief Train on the current training set.
     */
    virtual void train() = 0;

    /**
     * @brief Make a prediction using the given feature set.
     * @param fs The feature set to make a prediction on.
     * @return A bitmask indicating the classes chosen from the trainers used.
     */
    virtual int predict(FeatureSet &fs) = 0;

    /**
     * @brief usingStandardVideo
     * @return Whether or not this experiment is using standard video.
     */
    virtual bool usingStandardVideo() = 0;

};

#endif // ICONTROLCENTERMANAGER_H
