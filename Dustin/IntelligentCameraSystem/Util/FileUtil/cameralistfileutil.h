#ifndef CAMERALISTFILEUTIL_H
#define CAMERALISTFILEUTIL_H
#include <QString>
#include <ThirdParty/pugixml/pugixml.hpp>
#include "Types/camera.h"


using namespace pugi;
/**
 * @brief An XML file reading utility.
 *This class defines a utility for reading and writing an xml configuration file containing a list of saved camera servers.
 */
class CameraListFileUtil
{
public:
    /**
     * @brief Default value constructor.
     */
    CameraListFileUtil();

    /**
     * @brief Load the xml document.
     *This function will load the xml document into the pugixml backend.
     * @param filename The name of the xml file to load.
     * @return True if the document loaded, false otherwise.
     */
    bool loadDocument(QString &filename);

    /**
     * @brief Get the camera list.
     *Parses the loaded xml document and returns a list of cameras.
     * @return A list of cameras specified by the XML document.
     */
    CameraList getCameraList();

private:
    //Main document node
    xml_document                 m_root;
    bool                         m_is_loaded;

    //Open document name
    QString                      m_open_document;
};

#endif // CAMERALISTFILEUTIL_H
