#include "learninginterface.h"
#include "Global/Log.h"
LearningInterface::LearningInterface() :
    m_trainer(svm_c_trainer<kernel_type>()),
    m_is_trained(false)
{
}

//Add a training sample
void LearningInterface::addTrainingSample(FeatureSet &features, double label)
{
    sample_type samp;
//    samp(0) = (double)features.bandwidth();
//    samp(1) = (double)features.contentType();
    for(int i = 0; i < FEATURE_SET_NUM_FEATURES; i++)
        samp(i) = features.getFeatureByIndex((FeatureSetIndexType) i);

    m_training_set.push_back(samp);
    if(label == 1.0)
        m_labels.push_back(+1);
    else
        m_labels.push_back(-1);
}

void LearningInterface::trainCurrent()
{
    //Normalize the training set.
    m_normalizer.train(m_training_set);
    for (unsigned long i = 0; i < m_training_set.size(); ++i)
        m_training_set[i] = m_normalizer(m_training_set[i]);

    //Perform cross-validation over a range of values of C and gamma
    //to determine the best parameters
    double C_best = 0;
    double gamma_best = 0;
    double best_accuracy = 0.0;

    //Different values, simple implementation
    //Could look into better methods of doing this but this is a nice
    //and simple implementation at the cost of speed and efficiency
    for(double gamma = 0.0001; gamma < 3; gamma *= 5)
    {
        for(double C = 0.0001; C < 3; C *= 5)
        {
            //Set a new kernel
            m_trainer.set_kernel(kernel_type(gamma));

            //Set a new C
            m_trainer.set_c(C);

            //Perform 3-fold cross validation and look at the accuracy
            matrix<double, 1, 2> accuracy = cross_validate_trainer(m_trainer, m_training_set, m_labels, 3);
            DEBUG() << " cross-validation accuracy: " << accuracy(0,0) << " " << accuracy(0,1);
            //See if this has the best accuracy so far by multiplying the 0 class accuracy and 1 class accuracy
            double curr_accuracy = accuracy(0,0) * accuracy(0,1);
            if(curr_accuracy > best_accuracy)
            {
                best_accuracy = curr_accuracy;
                C_best = C;
                gamma_best = gamma;
            }

        }
    }

    //Set C, epsilon and cache size
    m_trainer.set_c(C_best);

    //For now just leave defaults

    //Set gamma
    m_trainer.set_kernel(kernel_type(gamma_best));
    m_decision_function.normalizer = m_normalizer;

    //Train
    m_decision_function.function = m_trainer.train(m_training_set, m_labels);

    m_is_trained = true;

}

//Make a prediction using the given feature set
double LearningInterface::predict(FeatureSet &features)
{
    //Get the sample
    sample_type sample;
//    sample(0) = (double)features.bandwidth();
//    sample(1) = (double)features.contentType();
    for(int i = 0; i < FEATURE_SET_NUM_FEATURES; i++)
        sample(i) = features.getFeatureByIndex((FeatureSetIndexType) i);

    return m_decision_function(sample);

}
