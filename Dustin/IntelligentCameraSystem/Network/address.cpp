#include <strstream>
#include <string>
#include "Network/address.h"


Address::Address()
{
    this->m_port = 0;
    this->m_address = "127.0.0.1";
}
Address::Address(string address, int port) :
    m_address(address),
    m_port(port)
{}

string Address::getAddress() const
{
    return this->m_address;
}

int Address::getPort() const
{
    return this->m_port;
}

const char *Address::getStringPort() const
{
    strstream ss;
    ss << this->m_port;
    return ss.str();
}

//Operator overloads
bool Address::operator ==(const Address &rhs) const
{
    bool eq = true;

    if(this->m_address.compare(rhs.m_address) != 0)
    {
        eq = false;
    }

    if(this->m_port != rhs.m_port)
    {
        eq = false;
    }

    return eq;
}

bool Address::operator !=(const Address &rhs) const
{
    bool n_eq = true;

    if(this->m_address.compare(rhs.m_address) == 0)
    {
        n_eq = false;
    }

    if(this->m_port == rhs.m_port)
    {
        n_eq = false;
    }

    return n_eq;
}

void Address::setAddress(const string &address)
{
    m_address = address;
}

void Address::setPort(int port)
{
    m_port = port;
}
