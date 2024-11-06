#include "server.h"
#include <iostream>
#include <stdexcept>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>

Server::Server(int port)
    : hasClient (false)
{
    // Create socket
    socketId = socket (AF_INET, SOCK_STREAM, 0);

    if (socketId == -1)
        throw std::logic_error ("Could not create socket.");

    // Bind to ip and port
    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons (port);
    hint.sin_addr.s_addr = htonl (INADDR_ANY);

    *((uint64_t*)hint.sin_zero) = 0;

    if (bind (socketId, (sockaddr*)&hint, sizeof (hint)) == -1)
        throw std::logic_error ("Failed to bind socket.");

    std::cout << "Listening on: " << hint.sin_addr.s_addr << std::endl;

    if (listen (socketId, SOMAXCONN) == -1)
        throw std::logic_error ("Failed to listen.");
}

Server::~Server()
{
    close (socketId);

    if (hasClient)
        close (clientSocket);
}

void Server::waitForClient()
{
    if (hasClient)
        throw std::logic_error ("Server already has a client");

    sockaddr_in clientAddr;
    socklen_t clientSize = sizeof (clientAddr);

    clientSocket = accept (socketId, (sockaddr*)&clientAddr, &clientSize);

    if (clientSocket == -1)
        throw std::logic_error ("Error while accepting client");

    hasClient = true;
}

size_t Server::read(char* buffer, size_t bufferSize)
{
    return recv (clientSocket, buffer, bufferSize, 0);
}

void Server::write (const char* buffer, size_t bufferSize)
{
    send (clientSocket, buffer, bufferSize, 0);
}
