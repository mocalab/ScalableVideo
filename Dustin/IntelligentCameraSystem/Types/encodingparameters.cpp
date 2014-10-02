#include "encodingparameters.h"

EncodingParameters::EncodingParameters() :
    m_width(QString::number(352)),
    m_height(QString::number(288)),
    m_fps(QString::number(30)),
    m_bitrate(QString::number(352*288*3.5))
{
}

//Copy constructor
EncodingParameters::EncodingParameters(EncodingParameters &cpy)
{
    this->m_width = cpy.m_width;
    this->m_height = cpy.m_height;
    this->m_fps = cpy.m_fps;
    this->m_bitrate = cpy.m_bitrate;
}
//Assignment operator
EncodingParameters &EncodingParameters::operator=(EncodingParameters &rhs)
{
    this->m_width = rhs.m_width;
    this->m_height = rhs.m_height;
    this->m_fps = rhs.m_fps;
    this->m_bitrate = rhs.m_bitrate;

    return *this;
}

EncodingParameters::EncodingParameters(QString width, QString height, QString fps, QString bitrate)
{
    this->m_width = width;
    this->m_height = height;
    this->m_fps = fps;
    this->m_bitrate = bitrate;
}

QString EncodingParameters::width() const
{
    return m_width;
}

void EncodingParameters::setWidth(const QString &width)
{
    m_width = width;
}
QString EncodingParameters::height() const
{
    return m_height;
}

void EncodingParameters::setHeight(const QString &height)
{
    m_height = height;
}
QString EncodingParameters::bitrate() const
{
    return m_bitrate;
}

void EncodingParameters::setBitrate(const QString &bitrate)
{
    m_bitrate = bitrate;
}

int EncodingParameters::widthAsInt() const
{
    return this->m_width.toInt();
}

int EncodingParameters::heightAsInt() const
{
    return this->m_height.toInt();
}

int EncodingParameters::fpsAsInt() const
{
    return this->m_fps.toInt();
}

int EncodingParameters::bitrateAsInt() const
{
    QString bitrate = this->m_bitrate;

    if(this->m_bitrate.contains('\''))
        bitrate.chop(1);

    return bitrate.toInt();
}

//Equivalence operator
bool EncodingParameters::operator==(const EncodingParameters &rhs)
{
    return (this->m_width == rhs.m_width && this->m_height == rhs.m_height
            && this->m_fps == rhs.m_fps && this->m_bitrate == rhs.m_bitrate);
}
QString EncodingParameters::fps() const
{
    return m_fps;
}

void EncodingParameters::setFps(const QString &fps)
{
    m_fps = fps;
}











