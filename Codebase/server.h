#ifndef SERVER_H
#define SERVER_H

#include <sys/socket.h>

class Server
{
public:
    Server (int port);
    ~Server ();

    void waitForClient ();

    size_t read (char* buffer, size_t bufferSize);
    void write (const char* buffer, size_t bufferSize);
    
private:
    int socketId;
    bool hasClient;
    int clientSocket;
};

#endif // SERVER_H
