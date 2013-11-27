#ifndef BANDWIDTHFILEREADER_H
#define BANDWIDTHFILEREADER_H

#define  BWFILE_PATH            "../IntelligentCameraSystem/Config/"

#include <QObject>
#include <QMutex>
#include "filereaderutility.h"

/**
 * @brief A TEST class to read a file that will contain bandwidth information.
 *This class is used for testing purposes for reading a file that will contain information about available channel bandwidth.
 *The class will open the file and read lines as they become available that will write the new value of the bandiwdth in Kbps.
 */
class BandwidthFileReader : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief Explicit value contructor.
     * @param filename The name of the bandwidth information file.
     */
    BandwidthFileReader(QString &filename);

    /**
     * @brief Set the value of the ready flag.
     * @param val The new value of the read flag.
     */
    void setReadyFlag(bool val);

signals:
    /**
     * @brief Slot to signal a new bandwidth is available.
     * @param bandwidth The new bandwidth.
     */
    void newBandwidth(QString bandwidth);
    /**
     * @brief Signal emitted when work has completed.
     */
    void finished();
public slots:
    /**
     * @brief The readLoop slot.
     *This function will continuously read the bandwidth information file until EOF is found or the ready
     *flag is set to false.
     */
    void readLoop();
private:

    FileReaderUtility           m_filereader; /**< The file reader*/

    bool                        m_ready;    /**< A flag to indicate this object is ready to run */

    QMutex                      m_readymutex; /**< A mutex for the ready flag */


};

#endif // BANDWIDTHFILEREADER_H
