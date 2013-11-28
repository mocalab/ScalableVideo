#ifndef STDTYPES_H
#define STDTYPES_H
#include <list>
#include <string>
///List typedefs for the older UI (not used anymore)
namespace MainWindowUI{

    //List type for command memory
    typedef std::string                            CommandListItem;
    typedef std::list<CommandListItem>             CommandList;
    typedef std::list<CommandListItem>::iterator   CommandListIterator;

}



#endif // STDTYPES_H
