#include "../../includes/Server.h"
#include <ctime>
#include <errno.h>
#include <fcntl.h>
#include <iostream>
#include <netinet/in.h>
#include <stdexcept>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <vector>

Server::Server(int port, std::string password)
    : _port(port),
      _password(password),
      _server_fd(0),
      _clients(0),
      _fds(0),
      _creationTime(std::time(0)),
      _commandHandler(CommandHandler()) {
    setupServerSocket();
}

Server::~Server() {
    for (size_t i = 0; i < _fds.size(); ++i) {
        close(_fds[i].fd);
    }
}

static void setNonBlocking(int fd) {
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags == -1) {
        throw std::runtime_error("Failed to get socket flags: " + std::string(strerror(errno)));
    }
    if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) == -1) {
        throw std::runtime_error("Failed to set socket to non-blocking mode: " +
                                 std::string(strerror(errno)));
    }
}

void Server::setupServerSocket() {
    _server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (_server_fd == -1) {
        throw std::runtime_error("Failed to create server socket");
    }

    int optval = 1;
    if (setsockopt(_server_fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) == -1) {
        throw std::runtime_error("Failed to set socket option SO_REUSEADDR");
    }

    struct sockaddr_in serverAddress;
    memset(&serverAddress, 0, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(_port);

    if (bind(_server_fd, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) == -1) {
        throw std::runtime_error("Failed to bind server socket to port");
    }

    if (listen(_server_fd, SOMAXCONN) == -1) {
        throw std::runtime_error("Failed to listen on server socket");
    }

    setNonBlocking(_server_fd);

    pollfd server_pfd;
    server_pfd.fd = _server_fd;
    server_pfd.events = POLLIN;
    _fds.push_back(server_pfd);
}

static Client *getClientByFd(std::vector<Client *> clients, int fd) {
    for (int i = 0; i < (int)clients.size(); i++) {
        if (clients[i]->getFd() == fd)
            return clients[i];
    }
    return NULL;
}
void Server::run() {
    std::cout << "Server running. Listening on port " << _port << std::endl;

    while (true) {
        int pollResult = poll(_fds.data(), _fds.size(), -1);

        if (pollResult == -1) {
            throw std::runtime_error("Error in poll: " + std::string(strerror(errno)));
        }

        for (size_t i = 0; i < _fds.size(); ++i) {
            if (_fds[i].revents & POLLIN) {
                if (_fds[i].fd == _server_fd) {
                    struct sockaddr_in clientAddress;
                    socklen_t          clientAddressLength = sizeof(clientAddress);
                    int                clientSocket =
                        accept(_server_fd, (struct sockaddr *)&clientAddress, &clientAddressLength);

                    if (clientSocket == -1) {
                        std::cerr << "Failed to accept incoming connection: " << strerror(errno)
                                  << std::endl;
                        continue;
                    }

                    Client *newClient = new Client(clientSocket, this);
                    _clients.push_back(newClient);

                    pollfd client_pfd;
                    client_pfd.fd = clientSocket;
                    client_pfd.events = POLLIN;
                    _fds.push_back(client_pfd);

                    std::cout << "New client connected to the server" << std::endl;
                } else {
                    // on a un client qui a envoyé un message
                    // on doit le lire et le traiter
                    char buffer[1024];
                    int  bytesRead = read(_fds[i].fd, buffer, sizeof(buffer));

                    if (bytesRead == -1) {
                        std::cerr << "Failed to read from client socket: " << strerror(errno)
                                  << std::endl;
                        continue;
                    }

                    if (bytesRead == 0) {
                        // client a fermé la connexion
                        close(_fds[i].fd);
                        _fds.erase(_fds.begin() + i);
                        delete _clients[i];
                        _clients.erase(_clients.begin() + i);
                        continue;
                    }

                    std::string message(buffer, bytesRead);
                    std::cout << "Received message from client: " << message << std::endl;

                    // on traite le message
                    _commandHandler.handleCommand(getClientByFd(_clients, _fds[i].fd), message);
                }
            }
        }
    }
}
