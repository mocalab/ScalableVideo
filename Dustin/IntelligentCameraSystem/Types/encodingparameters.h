/**
 *  @file Definition of the EncodingParameters class.
 */
#ifndef ENCODINGPARAMETERS_H
#define ENCODINGPARAMETERS_H
#include <iostream>
#include <QString>
/**
 * @brief A type encapsulating the tunable parameters of the encoder.
 */
class EncodingParameters
{
public:
    /**
     * @brief Default value constructor.
     */
    EncodingParameters();

    /**
     * @brief Copy constructor.
     * @param cpy The parameters to copy.
     */
    EncodingParameters(EncodingParameters& cpy);

    /**
     * @brief Overloaded assignment operator.
     * @param rhs The EncodingParameters object to copy over.
     * @return A reference to this object.
     */
    EncodingParameters &operator=(EncodingParameters &rhs);

    /**
     * @brief Explicit value constructor.
     * @param width
     * @param height
     * @param fps
     * @param bitrate
     */
    EncodingParameters(QString width, QString height, QString fps, QString bitrate);



    QString width() const;
    void setWidth(const QString &width);

    QString height() const;
    void setHeight(const QString &height);

    QString fps() const;
    void setFps(const QString &fps);

    QString bitrate() const;
    void setBitrate(const QString &bitrate);

    //Getters as integers
    int widthAsInt() const;

    int heightAsInt() const;

    int fpsAsInt() const;

    int bitrateAsInt() const;



private:

    QString             m_width;
    QString             m_height;
    QString             m_fps;
    QString             m_bitrate;
};

#endif // ENCODINGPARAMETERS_H
