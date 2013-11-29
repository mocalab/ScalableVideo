/**
 *  @file Constants for setting up basic sockets for testing.
 */

#ifndef SOCKETCONSTANTS_H
#define SOCKETCONSTANTS_H

///Receiver's address
#define RECEIVER_ADDRESS            "127.0.0.1"
///Receiver's listening port
#define RECEIVER_PORT               37986

///Port we are listening for messages on
#define LISTEN_PORT                 7744

///Port we are sending messaged through
#define SEND_PORT                   7745
#define STR_SEND_PORT               "7745"

///Backlog for incoming connections
#define BACKLOG                     2

#endif // SOCKETCONSTANTS_H
