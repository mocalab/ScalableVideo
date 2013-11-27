#ifndef ADDRESS_H
#define ADDRESS_H

#include <string>

using namespace std;
/**
 * @brief Address type
 *This class defines a type for a network address. It encapsulates two fields for the IP Address and the port to
 *communicate over.
 */
class Address
{
public:
    /**
     * @brief Default constructor.
     */\
    Address();
    /**
     * @brief Explicit value constructor.
     * @param address The IP address in string form.
     * @param port  The port in integer form
     */
    Address(string address, int port);

    /**
     * @brief IP Address accessor.
     *Accessor for the m_address field.
     * @return The IP address this object represents.
     */
    string getAddress() const;
    /**
     * @brief Port number accessor.
     *Accessor for the m_port field.
     * @return The port for this address.
     */
    int getPort() const;

    /**
     * @brief Port number accessor.
     *Accessor for the m_port field that will return a string representation of the port.
     * @return A C string contaning the port number.
     */
    const char *getStringPort() const;

    /**
     * @brief operator ==.
     *Overloaded equivalence operator.
     * @param rhs The address to compare to.
     * @return True if the addresses have the same address and port, false otherwise.
     */
    bool operator==(const Address& rhs) const;
    /**
     * @brief operator !=.
     *Overloaded not-equals operator.
     * @param rhs The address to compare to.
     * @return True if this object and rhs do not have the same address and port number, false otherwise.
     */
    bool operator!=(const Address& rhs) const;

    /**
     * @brief IP address mutator.
     *Mutator for the m_address field.
     * @param address The new address this object will represent.
     */
    void setAddress(const string &address);
    /**
     * @brief Port number mutator.
     *Mutator for the m_port field.
     * @param port The new port for this address
     */
    void setPort(int port);

private:

    string              m_address; /**< The address this object represents.*/

    int                 m_port; /**< m_port field -- The port for this address.*/
};

#endif // ADDRESS_H
