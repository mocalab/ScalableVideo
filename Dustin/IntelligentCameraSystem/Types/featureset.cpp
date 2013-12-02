#include "featureset.h"

FeatureSet::FeatureSet() :
    m_bandwidth(5000),
    m_contentType(0)
{
    m_feature_array[FEATURE_SET_idxBandwidth] = (double) m_bandwidth;
    m_feature_array[FEATURE_SET_idxContentType] = (double) m_contentType;
}

FeatureSet::FeatureSet(FeatureSet &cpy)
{
    this->m_bandwidth = cpy.m_bandwidth;
    this->m_contentType = cpy.m_contentType;

    m_feature_array[FEATURE_SET_idxBandwidth] = (double) cpy.m_bandwidth;
    m_feature_array[FEATURE_SET_idxContentType] = (double) cpy.m_contentType;
}

FeatureSet &FeatureSet::operator=(FeatureSet &rhs)
{
    this->m_bandwidth = rhs.m_bandwidth;
    this->m_contentType = rhs.m_contentType;

    m_feature_array[FEATURE_SET_idxBandwidth] = (double) rhs.m_bandwidth;
    m_feature_array[FEATURE_SET_idxContentType] = (double) rhs.m_contentType;
    return *this;
}

FeatureSet::FeatureSet(int bandwidth, int contentType) :
    m_bandwidth(bandwidth),
    m_contentType(contentType)
{
    m_feature_array[FEATURE_SET_idxBandwidth] = (double) m_bandwidth;
    m_feature_array[FEATURE_SET_idxContentType] = (double) m_contentType;
}

//Get a specific feature based on its index in the feature set array
double FeatureSet::getFeatureByIndex(FeatureSetIndexType idx)
{
    return m_feature_array[idx];
}
int FeatureSet::bandwidth() const
{
    return m_bandwidth;
}

void FeatureSet::setBandwidth(int bandwidth)
{
    m_bandwidth = bandwidth;
    m_feature_array[FEATURE_SET_idxBandwidth] = (double) bandwidth;
}
int FeatureSet::contentType() const
{
    return m_contentType;

}

void FeatureSet::setContentType(int contentType)
{
    m_contentType = contentType;
    m_feature_array[FEATURE_SET_idxContentType] = (double) contentType;
}


