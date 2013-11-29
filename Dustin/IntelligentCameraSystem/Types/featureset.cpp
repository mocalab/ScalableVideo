#include "featureset.h"

FeatureSet::FeatureSet() :
    m_bandwidth(5000),
    m_contentType(0)
{
}

FeatureSet::FeatureSet(FeatureSet &cpy)
{
    this->m_bandwidth = cpy.m_bandwidth;
    this->m_contentType = cpy.m_contentType;
}

FeatureSet &FeatureSet::operator=(FeatureSet &rhs)
{
    this->m_bandwidth = rhs.m_bandwidth;
    this->m_contentType = rhs.m_contentType;

    return *this;
}

FeatureSet::FeatureSet(int bandwidth, int contentType) :
    m_bandwidth(bandwidth),
    m_contentType(contentType)
{

}
int FeatureSet::bandwidth() const
{
    return m_bandwidth;
}

void FeatureSet::setBandwidth(int bandwidth)
{
    m_bandwidth = bandwidth;
}
int FeatureSet::contentType() const
{
    return m_contentType;
}

void FeatureSet::setContentType(int contentType)
{
    m_contentType = contentType;
}


