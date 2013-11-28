#include "cameralistfileutil.h"

#include <Global/Log.h>

CameraListFileUtil::CameraListFileUtil() :
    m_is_loaded(false),
    m_open_document("")
{
}

//Load the xml document
bool CameraListFileUtil::loadDocument(QString &filename)
{
    bool is_loaded = false;

    //Load the file
    xml_parse_result result = m_root.load_file(filename.toStdString().c_str());

    if(result)
    {
        //Success
        is_loaded = true;
        this->m_is_loaded = true;

        this->m_open_document = filename;
    }
    else
    {
        L_ERROR() << "Unable to load XML file: " << filename;
    }

    return is_loaded;

}

//Parse an open xml document
CameraList CameraListFileUtil::getCameraList()
{
    CameraList cameras;

    //See if document is loaded
    if(!m_is_loaded)
        return cameras;

    //Parse the XML

    //Load all cameras
    xml_node cams = m_root.child("cameraList").child("camera");

    //Iterate through all cameras
    for(xml_node cam = cams; cam; cam = cam.next_sibling())
    {
        //NOTE --- RELYING ON THE UI TO DELETE ALLOCATED RESOURCES
        Camera *next_camera = new Camera();
        //Get the name
        xml_attribute name = cam.attribute("name");
        next_camera->setName(QString(name.as_string()));



        //Get the address
        Address next_addr;
        xml_node addr = cam.child("address");
        xml_attribute port = addr.attribute("port");
        next_addr.setAddress(std::string(addr.text().as_string()));
        next_addr.setPort(port.as_int());

        next_camera->setServer_address(next_addr);
        //Get the content type
        xml_node content_type = cam.child("contentType");
        next_camera->setContent_type((CameraContentType)content_type.attribute("enum").as_int());

        //Add to camera list
        cameras.push_front(next_camera);
    }

    return cameras;
}
