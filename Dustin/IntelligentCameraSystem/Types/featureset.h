#ifndef FEATURESET_H
#define FEATURESET_H

#define FEATURE_SET_NUM_FEATURES        2

/**
 *  @brief Enumeration of feature set array indexes.
 */
typedef enum {
    FEATURE_SET_idxBandwidth = 0,           /**< Index of the bandwidth feature. */
    FEATURE_SET_idxContentType              /**< Index of the content type feature. */
} FeatureSetIndexType;

/**
 * @brief This class defines a type encapsulating the features we are interested in tracking.
 */
class FeatureSet
{
public:
    /**
     * @brief Default value constructor.
     */
    FeatureSet();

    /**
     * @brief Copy constructor.
     * @param cpy
     */
    FeatureSet(FeatureSet &cpy);

    /**
     * @brief Assignment operator.
     * @param rhs
     * @return A reference to this object.
     */
    FeatureSet &operator=(FeatureSet &rhs);

    /**
     * @brief Explicit value constructor.
     * @param bandwidth
     * @param contentType
     */
    FeatureSet(int bandwidth, int contentType);

    /**
     * @brief Get the feature specified by its index in the feature set array.
     *This creates a simpler mechanism for retrieving the entire list of features. A caller can simply iterate over
     *all possible features instead of having to make individual calls to each feature's accessor.
     * @param idx The index of the feature to ve retrieved.
     * @return The value of the specific feature.
     */
    double getFeatureByIndex(FeatureSetIndexType idx);

    int bandwidth() const;
    void setBandwidth(int bandwidth);

    int contentType() const;
    void setContentType(int contentType);

private:
    int                     m_bandwidth;
    int                     m_contentType;

    //Keep an array with the feature data in it
    double                  m_feature_array[FEATURE_SET_NUM_FEATURES];

};

#endif // FEATURESET_H
