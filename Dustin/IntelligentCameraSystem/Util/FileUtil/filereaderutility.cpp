#include "filereaderutility.h"
#include <QTextStream>

FileReaderUtility::FileReaderUtility() :
    m_filename(""),
    m_isOpen(false),
    m_lines(QStringList()),
    m_infile(NULL),
    m_stream(NULL)
{

}

FileReaderUtility::FileReaderUtility(QString filename) :
    m_filename(filename),
    m_isOpen(false),
    m_lines(QStringList()),
    m_infile(NULL),
    m_stream(NULL)
{
}

FileReaderUtility::~FileReaderUtility()
{
    this->close();
}

void FileReaderUtility::setFile(const QString &filename)
{
    m_filename = filename;
}

QString FileReaderUtility::getFile() const
{
    return this->m_filename;
}

bool FileReaderUtility::open_file()
{
    //Open the file
    m_infile = new QFile(m_filename);
    m_isOpen = m_infile->open(QIODevice::ReadOnly);

    if(m_isOpen)
    {
        //Open a text stream;
        m_stream = new QTextStream(m_infile);
    }

    //Open read only
    return m_isOpen;

}

//Close the file
void FileReaderUtility::close()
{
    if(this->isOpen())
    {
        m_infile->close();
        m_isOpen = false;

        //Delete resources
        delete m_infile;
        m_infile = NULL;
        delete m_stream;
        m_stream = NULL;
    }
}

bool FileReaderUtility::isOpen()
{
    return m_isOpen;
}

QStringList &FileReaderUtility::parse()
{
    //See if the file is open
    if(!m_isOpen)
        return m_lines;

    //Create an input stream
    QTextStream input_stream(m_infile);

    //Parse each line
    while(!input_stream.atEnd())
    {
        //Get next line
        QString next = input_stream.readLine();
        m_lines << next;

    }

    this->close();

    return m_lines;

}

//Read the next line of the file
QString FileReaderUtility::readLine()
{
    QString next("");
    //See if the file is open
    if(!m_isOpen)
        return next;

    //Create an input stream
    //QTextStream input_stream(m_infile);

    if(this->isOpen())
    {
        //Read the next line
        next = m_stream->readLine();
        m_lines << next;

    }
    else
        this->close();

    return next;

}
