#include <Global/Log.h>
#include <fstream>
#include "serverconfigfileutil.h"

ServerConfigFileUtil::ServerConfigFileUtil() :
    m_is_loaded(false)
{
}

bool ServerConfigFileUtil::loadDocument(const char *file_name)
{
    bool loaded = false;
    //Load the file
    //std::ifstream stream(file_name);
    xml_parse_result result = m_root.load_file(file_name);
    if(result)
    {
        loaded = true;
        m_is_loaded = true;
        //strcpy(m_open_document, file_name);
    }
    else
    {
        L_ERROR() << "Unable to load xml file. Result: " << result.description();
    }

    return loaded;
}

//Gets all servers in the xml file and returns them in a map
AddressList ServerConfigFileUtil::getServerInfo()
{
    AddressList server_list;
    if(!m_is_loaded)
    {
        L_ERROR() << "Document not loaded!";
        return server_list;
    }
    //get all the "server" nodes
    xml_node servers = m_root.child("serverConfig").child("server");
    //iterate over all servers
    for(xml_node server = servers; server; server = server.next_sibling())
    {
        //get the host and port, place in map
        xml_attribute host = server.attribute("hostname");
        const char *hostname = host.as_string();

        //port
        xml_attribute port = server.attribute("port");
        int portnum = port.as_int();

        Address next(hostname, portnum);
        server_list.push_back(next);
    }

    return server_list;
}

//Save the supplies server info in the database
void ServerConfigFileUtil::saveServerInfo(AddressList info, const char *file_name)
{
//    //Reopen the file
    //bool result = this->loadDocument(m_open_document);
//    if(!result)
//    {
//        L_ERROR() << "Unable to load xml file. Result: ";
//        return;
//    }

    //Just using one server for now
    AddressListIter first = info.begin();
    //Set new node values
    xml_node servers = m_root.child("serverConfig").child("server");
    servers.attribute("hostname").set_value(first->getAddress().c_str());
    servers.attribute("port").set_value(first->getPort());

    INFO() << servers.attribute("hostname").value();

    bool saved = m_root.save_file(file_name);
}
