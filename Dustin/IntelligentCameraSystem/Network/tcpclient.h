/**
 *  @file Definition of a TCP client class and associated types.
 */

#ifndef TCPCLIENT_H
#define TCPCLIENT_H

class Address;

/**
 * @brief Enumeration for the TCPError type.
 */
enum TCPError
{
    TCP_STATUS_SUCCESS = 0,                 /**< No errors*/
    TCP_STATUS_ADDRESS_NOT_FOUND = 1,       /**< Could not find the specified server.*/
    TCP_STATUS_SOCKET_NOT_OPENED = 2,       /**< Could not open a socket.*/
    TCP_STATUS_CONNECTION_CLOSED = 3,       /**< Conection lost.*/
    TCP_STATUS_SEND_FAILED = 4,             /**< Failed to send a request.*/
    TCP_STATUS_RECEIVE_FAILED = 5,          /**< Failed to receive a response.*/
    TCP_STATUS_SERVER_DISCONNECTED = 6      /**< Server disconnected.*/
};

/**
 * @brief This class defines a TCP client that will send requests to and receive responses from a specified server.
 */
class TCPClient
{
public:
    /**
     * @brief Default value constructor.
     */
    TCPClient();

    /**
     * @brief Connect to a specified server.
     * @param server_addr The address of the server to connect to.
     * @return TCP_STATUS_SUCCESS on success, TCP_STATUS_ADDRESS_NOT_FOUND when the specified address could not be found, or TCP_STATUS_SOCKET_NOT_OPENED if a socket could not be opened.
     */
    TCPError connectToServer(Address server_addr);

    /**
     * @brief Disconnect from the server.
     */
    void disconnect();

    /**
     * @brief Send a request to the server.
     * @param send_data The data to send to the server.
     * @param size The size of the data to send.
     * @return TCP_STATUS_SUCCESS on success, TCP_STATUS_CONNECTION_CLOSED if not connected to a server, or TCP_STATUS_SEND_FAILED if the message fails to send.
     */
    TCPError Send(const void *send_data, int size);

    /**
     * @brief Receive a response from the server.
     * @param recv_data A buffer to store the received data in.
     * @param max The storage buffer size.
     * @return TCP_STATUS_SUCCESS on success, TCP_STATUS_CONNECTION_CLOSED if not connected to a server, TCP_STATUS_RECEIVE_FAILED if failed to receive a packet, or TCP_STATUS_SERVER_DISCONNECTED if the connection is closed on the server side.
     */
    TCPError Receive(char *recv_data, int max);

    //Check if socket is open
    /**
     * @brief Whether or not the socket is open.
     * @return True if the socket is open.
     */
    bool isOpen() const;
    /**
     * @brief Whether or not the object is connected to a server.
     * @return True if this object is connected to a server.
     */
    bool isConnected() const;

    //For implementing a server (if needed)

    //Open a socket
    bool openLocal();

    //Listen for incoming connections
    bool Listen();

private:
    //Socket file descriptors
    int         m_sockfd_local;
    int         m_sockfd_server;
    //flags
    bool        m_is_open;
    bool        m_is_connected;
};

#endif // TCPCLIENT_H
