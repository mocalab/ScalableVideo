#ifndef FEATURESET_H
#define FEATURESET_H

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

    int bandwidth() const;
    void setBandwidth(int bandwidth);

    int contentType() const;
    void setContentType(int contentType);

private:
    int                     m_bandwidth;
    int                     m_contentType;

};

#endif // FEATURESET_H
