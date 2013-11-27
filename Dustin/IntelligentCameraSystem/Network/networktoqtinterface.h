#ifndef NETWORKTOQTINTERFACE_H
#define NETWORKTOQTINTERFACE_H
#include <QObject>
#include <Util/tickingtimer.h>
#include "tcpclient.h"
#include "address.h"

/**
 * @brief The NetworkToQtInterface class.
 * An interface between Qt and the underlying TCP client. Objects of this class
 *generally should be moved into another thread before used by connecting a QThread object's
 *start signal with the procRequest() slot.
 */
class NetworkToQtInterface : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief Default value constructor.
     */
    NetworkToQtInterface();
    /**
     * @brief Explicit value constructor.
     * @param server_address The address to connect to.
     */
    NetworkToQtInterface(Address server_address);
    /**
    *   @brief Destructor.
    */
    ~NetworkToQtInterface();

    //Connect functions
    /**
     * @brief Connect to a server.
     *Connect to the server whose address is represented by the field m_server_address.
     * @return True if connection succeeded, false otherwise.
     */
    bool Connect(); //Using internally defined address
    /**
     * @brief Connect to a specific server.
     *Connect to the server represented by the parameter server_address.
     * @param server_address The address to connect to.
     * @return True if connection succeeded, false otherwise.
     */
    bool Connect(Address server_address); //Using new address


    /**
     * @brief Connection status.
     *Indicate whether or not this object is connected to a server.
     * @return True if connected to a server, false otherwise.
     */
    bool isConnected() const;

    /**
     * @brief Set the internal message buffer.
     *Set the internal buffer containing the message to be sent to the server. This
     *internal buffer is used when the procRequest() slot is invoked, generally when the
     *thread that an object of NetworkToQtInterface type is moved to issues it's start signal.
     * @param data The message to send to the server.
     */
    void setBuffer(QString &data);

    /**
     * @brief Set the server address.
     *Set the server address that this object should connect to when the Connect() call is made.
     * @param addr The address to set.
     */
    void setServerAddress(Address &addr);

    //Send a message to the server immediately
    /**
     * @brief Directly send a message.
     *Immediately send the message msg to the server this object is connected to, or send nothing if
     *not connected to a server.
     * @param msg The message to send
     */
    void sendMessage(const QString &msg);
public slots:
    //Methods to be called in another thread
    /**
     * @brief Process a request/response sequence.
     *A slot which will begin a request response sequence; A message is attempted to be sent to the server.
     *If successful, this function will wait until the server responds, and will emit a messageDispatch signal
     *with either the server's response or an error message.
     */
    void procRequest();
    //connection timeout slot
    /**
     * @brief Timeout the connection.
     *Slot which will set an internal flag indicating that the connection has timed out.
     */
    void connectionTimeout();
    //Close function
    /**
     * @brief Disconnect from server.
     *Slot which will disconnect from the server.
     */
    void close();
private slots:
    /**
     * @brief Listen for a server response.
     *Private slot that will check for a response from the server and exit if no response if available or
     *the server has disconnected. This slot is called internally by a timer of type Util/TickingTimer, which
     *will call this slot for a specified number of ticks.
     */
    void listenForResponse();

signals:
    //Signals called from another thread
    void receiveFinished();
    /**
     * @brief Dispatch a message.
     *Signal that is emitted when an event occurs in the procRequest() sequence that will send a message
     *containing information about the current state of execution (i.e. server response, error messages)
     * @param msg The message to send to connected handlers.
     */
    void messageDispatch(QString msg);
    //Finish thread signal
    /**
     * @brief Finish thread execution.
     *Signal to indicate end of execution of the procRequest() sequence.
     */
    void finished();
    //Server disconnected
    /**
     * @brief Server has disconnected.
     *Signal to indicate that the server has disconnected.
     */
    void serverDisconnected();


private:

    Address                     m_server_address; /**< The address of the server to connect to.*/

    TCPClient                   m_tcp_client; /**< The underlying TCP client to use for communication.*/

    bool                        m_connection_timeout; /**< Flag indicating the connection has timed out.*/

    QString                     m_buffer; /**< Internal buffer to be used as a message buffer.*/


    TickingTimer                *m_response_loop;/**< Timer which will keep track of the number of ticks; used to
                                                 determine when the response wait loop should time out.*/

    int                          m_ticks; /**< Keeps track of timer ticks*/
    //Functions
    /**
     * @brief Set up an internal timer.
     *Function which will set up the m_response_loop timer.
     */
    void setUpTimer();
};

#endif // NETWORKTOQTINTERFACE_H
