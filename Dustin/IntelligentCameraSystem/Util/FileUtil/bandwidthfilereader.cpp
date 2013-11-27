#include "bandwidthfilereader.h"
#include <sys/stat.h>

BandwidthFileReader::BandwidthFileReader(QString &filename) :
    m_filereader(FileReaderUtility())
{
    //Create the fifo and set the path
    QString fifo_name(BWFILE_PATH);
    fifo_name += filename;

    //Create the fifo
    int ready = mkfifo(fifo_name.toStdString().c_str(), S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH);

    //Set the ready flag
    if(ready == 0)
    {
        //Set the file reader file
        m_filereader.setFile(fifo_name);
        m_ready = true;
    }
    else
        m_ready = false;
}

BandwidthFileReader::~BandwidthFileReader()
{
    //Destroy the fifo
    unlink(m_filereader.getFile().toStdString().c_str());
}

//Sets the value of the ready flag
void BandwidthFileReader::setReadyFlag(bool val)
{
    m_readymutex.lock();
    this->m_ready = val;
    m_readymutex.unlock();
}

//A slot called from another thread to read the bandwidth information file.
void BandwidthFileReader::readLoop()
{
    //Loop while the ready flag is set

    //Try to open the file
    bool isReady = m_filereader.open_file();

    m_readymutex.lock();
    this->m_ready = isReady;
    m_readymutex.unlock();

    while(isReady)
    {
        //Read another line
        QString line = m_filereader.readLine();

        //If a null string
        if(!m_filereader.isOpen() || line == "-1")
        {
            //We've reached EOF
            m_readymutex.lock();
            this->m_ready = false;
            m_readymutex.unlock();
        }
        else
        {
            //Signal we have a new bandwidth
            if(line != "")
                emit newBandwidth(line);
        }

        //Set the new value of isReady
        //Lock the mutex
        m_readymutex.lock();
        //Set
        isReady = this->m_ready;
        //Unlock
        m_readymutex.unlock();
    }

    m_filereader.close();

    //Destroy the fifo
    unlink(m_filereader.getFile().toStdString().c_str());

    //Finished
    emit finished();
}
