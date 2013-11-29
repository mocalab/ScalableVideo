#ifndef SERVERCONFIGFILEUTIL_H
#define SERVERCONFIGFILEUTIL_H
#include <map>
#include <list>
#include <ThirdParty/pugixml/pugixml.hpp>
#include "Network/address.h"

typedef std::map<const char *, int>             HostPortMap;
typedef std::map<const char *, int>::iterator   HostPortMapIter;
typedef std::pair<const char *, int>            HostPortPair;

typedef std::list<Address>                      AddressList;
typedef std::list<Address>::iterator            AddressListIter;

using namespace pugi;

/**
 * @brief This class defines a utility for reading a configuration file for a server configuration.
 * @deprecated This class was used in the old version of the UI.
 */
class ServerConfigFileUtil
{
public:
    ServerConfigFileUtil();

    //Load the document
    bool loadDocument(const char *file_name);

    //Get all servers
    AddressList getServerInfo();

    //save server config
    void saveServerInfo(AddressList info, const char *file_name);
private:
    //Main document node
    xml_document          m_root;
    bool                  m_is_loaded;

    //Open document name
    char                  *m_open_document;
};

#endif // SERVERCONFIGFILEUTIL_H
