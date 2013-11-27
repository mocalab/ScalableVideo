#ifndef FILEREADERUTILITY_H
#define FILEREADERUTILITY_H
#include <QString>
#include <QStringList>
#include <QFile>

//Forward class declarations
class QTextStream;

/**
 * @brief A class to parse a file, specifically to generate a list of strings which are the lines of that file.
 */
class FileReaderUtility
{
public:

    /**
     * @brief Default value contructor.
     */
    FileReaderUtility();

    /**
     * @brief Explicit value constructor.
     * @param filename The file to read from.
     */
    FileReaderUtility(QString filename);

    ~FileReaderUtility();

    /**
     * @brief setFile
     *Sets the current filename to open in the open_file() call
     * @param filename The name of the file
     */
    void setFile(const QString& filename);

    /**
     * @brief Get the file path.
     * @return The path to the file in use.
     */
    QString getFile() const;

    /**
     * @brief Open the file.
     *This function will open the file specified by the m_filename field.
     * @return true if the file successfully opened, false otherwise.
     */
    bool open_file();

    /**
     * @brief Close the file.
     */
    void close();

    /**
     * @brief isOpen
     *Whether or not a file is open.
     * @return true if the file is open
     */
    bool isOpen();

    /**
     * @brief parse
     *This function will parse the opened file and return a list of strings (each line in the file)
     * @return A list of strings (each line in the file)
     */
    QStringList& parse();

    /**
     * @brief Read one line of the file.
     * @return The next line of the file to read.
     */
    QString readLine();

private:
    ///The name of the file
    QString                 m_filename;
    ///The file we are opening
    QFile                  *m_infile;

    ///A text stream for reading from the file
    QTextStream             *m_stream;

    ///A list of strings (each line of the input file)
    QStringList             m_lines;
    ///Whether or not the file is open
    bool                    m_isOpen;

};

#endif // FILEREADERUTILITY_H
