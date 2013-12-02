/**
 *  @file This file contains the class and associated types for the machine learning interface.
 *Note that many of the techniques used in this class are taken from examples created by dlib. See
 * http://dlib.net/svm_ex.cpp.html
 */
#ifndef LEARNINGINTERFACE_H
#define LEARNINGINTERFACE_H
#include "Types/featureset.h"
#include <dlib/svm.h>

using namespace dlib;


//Typedefs
typedef matrix<double, FEATURE_SET_NUM_FEATURES, 1>         sample_type;

//Radial basis kernel typedef
typedef radial_basis_kernel<sample_type>        kernel_type;

//Decision function
typedef decision_function<kernel_type>          dec_func_type;

//Normalized decision function
typedef normalized_function<dec_func_type>      learned_function;



/**
 * @brief This class encapsulates the machine learning mechanisms for determining user profiles.
 */
class LearningInterface
{
public:
    /**
     * @brief Default value contructor.
     */
    LearningInterface();

    /**
     * @brief Add a training sample.
     *Add a training sample with the specific features and label.
     * @param features The features of the sample.
     * @param label The label for this sample.
     */
    void addTrainingSample(FeatureSet &features, double label);

    /**
     * @brief Train this learning interface and generate a decision function.
     */
    void trainCurrent();

    /**
     * @brief Predict the class based on the feature set.
     * @param features The features.
     * @return 0 or 1
     */
    double predict(FeatureSet &features);

private:

    //Training set
    std::vector<sample_type>                    m_training_set;

    //Labels
    std::vector<double>                         m_labels;

    //The support vector machine
    svm_c_trainer<kernel_type>                  m_trainer;

    //A normalizer
    vector_normalizer<sample_type>              m_normalizer;

    //The decision function output
    learned_function                            m_decision_function;
    //Indicate if the trainer has been trained and we have a decision function
    bool                                        m_is_trained;

};

#endif // LEARNINGINTERFACE_H
