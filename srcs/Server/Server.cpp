#include "../../includes/Server.h"
#include <algorithm>
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

Server::Server(int port, const std::string &password)
    : _port(port),
      _password(password),
      _server_fd(0),
      _clients(0),
      _fds(0),
      _creationTime(std::time(0)),
      _commandHandler(this) {
    setupServerSocket();
}

Server::~Server() {
    for (size_t i = 0; i < _clients.size(); ++i) {
        delete _clients[i];
    }
    _clients.clear();

    for (std::map<std::string, Channel *>::iterator it = _channels.begin(); it != _channels.end();
         ++it) {
        delete it->second;
    }
    _channels.clear();

    if (_server_fd != -1) {
        close(_server_fd);
    }
}

static void setNonBlocking(int fd) {
    int flags = fcntl(fd, F_SETFL, O_NONBLOCK);
    if (flags == -1) {
        throw std::runtime_error("Failed to get socket flags: " + std::string(strerror(errno)));
    }
    if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) == -1) {
        throw std::runtime_error("Failed to set socket to non-blocking mode: " +
                                 std::string(strerror(errno)));
    }
}

std::string Server::getPassword() { return _password; }

std::vector<Client *> Server::getClients() { return _clients; }

std::map<std::string, Channel *> Server::getChannels() { return _channels; }

void Server::setShutdownFlag(bool value) { _shutdown = value; }

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
    serverAddress.sin_port = htons(static_cast<uint16_t>(_port));

    if (bind(_server_fd, reinterpret_cast<struct sockaddr *>(&serverAddress),
             sizeof(serverAddress)) == -1) {
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
    for (size_t i = 0; i < clients.size(); i++) {
        if (clients[i]->getFd() == fd)
            return clients[i];
    }
    return NULL;
}

void Server::removeClient(Client *client) {
    std::vector<Client *>::iterator it = std::find(_clients.begin(), _clients.end(), client);
    if (it != _clients.end()) {
        _clients.erase(it);
    }

    for (std::vector<pollfd>::iterator it2 = _fds.begin(); it2 != _fds.end(); ++it2) {
        if (it2->fd == client->getFd()) {
            close(it2->fd);
            _fds.erase(it2);
            break;
        }
    }

    delete client;
    client = NULL;
}

void Server::run() { // TODO : refactor this function
    std::cout << "Server running. Listening on port " << _port << std::endl;
    _shutdown = false;

    while (!_shutdown) {
        int pollResult = poll(_fds.data(), _fds.size(), -1);

        if (pollResult == -1) {
            if (errno == EINTR) {
                continue;
            }
            throw std::runtime_error("Error in poll: " + std::string(strerror(errno)));
        }

        if (_fds[0].revents & POLLIN) {
            struct sockaddr_in clientAddress;
            socklen_t          clientAddressLength = sizeof(clientAddress);
            int                clientSocket =
                accept(_server_fd, reinterpret_cast<struct sockaddr *>(&clientAddress),
                       &clientAddressLength);

            if (clientSocket == -1) {
                std::cerr << "Failed to accept incoming connection: " << strerror(errno)
                          << std::endl;
                continue;
            }

            setNonBlocking(clientSocket);

            Client *newClient = new Client(clientSocket, this);
            _clients.push_back(newClient);

            pollfd client_pfd;
            client_pfd.fd = clientSocket;
            client_pfd.events = POLLIN;
            client_pfd.revents = 0;
            _fds.push_back(client_pfd);

            std::cout << "New client connected to the server" << std::endl;
        }

        for (size_t i = 1; i < _fds.size(); ++i) {
            if (_fds[i].revents & POLLIN) {
                Client *currentClient = getClientByFd(_clients, _fds[i].fd);
                if (currentClient) {
                    char    buffer[1024];
                    ssize_t bytesRead = read(_fds[i].fd, buffer, sizeof(buffer));

                    if (bytesRead > 0) {
                        currentClient->appendToMessageBuffer(
                            std::string(buffer, static_cast<size_t>(bytesRead)));

                        size_t crlfPos;
                        while ((crlfPos = currentClient->getMessageBuffer().find("\r\n")) !=
                               std::string::npos) {
                            std::string message =
                                currentClient->getMessageBuffer().substr(0, crlfPos);
                            currentClient->removeFromMessageBuffer(crlfPos + 2);
                            _commandHandler.handleCommand(currentClient, message);
                            currentClient = getClientByFd(_clients, _fds[i].fd);
                            if (!currentClient)
                                break;
                        }
                    } else if (bytesRead == 0) {
                        std::string clientNickname = currentClient->getNickname();
                        if (clientNickname == "*") {
                            clientNickname = "unknown";
                        }

                        std::cout << "Client " << clientNickname << " disconnected" << std::endl;

                        std::map<std::string, Channel *> channels = currentClient->getChannels();
                        for (std::map<std::string, Channel *>::iterator it = channels.begin();
                             it != channels.end(); ++it) {
                            Channel *channel = it->second;
                            channel->removeMember(currentClient);
                        }
                        removeClient(currentClient);
                        --i;
                    } else {
                        std::cerr << "Error receiving data from client "
                                  << (currentClient->getNickname() != "*"
                                          ? currentClient->getNickname()
                                          : "unknown")
                                  << ": " << strerror(errno) << std::endl;
                    }
                }
            }
            if (_fds[i].revents & POLLHUP) {
                Client *currentClient = getClientByFd(_clients, _fds[i].fd);
                if (currentClient) {
                    std::string clientNickname = currentClient->getNickname();
                    if (clientNickname == "*") {
                        clientNickname = "unknown";
                    }

                    std::cout << "Client " << clientNickname << " disconnected (POLLHUP)"
                              << std::endl;

                    std::map<std::string, Channel *> channels = currentClient->getChannels();
                    for (std::map<std::string, Channel *>::iterator it = channels.begin();
                         it != channels.end(); ++it) {
                        Channel *channel = it->second;
                        channel->removeMember(currentClient);
                    }

                    removeClient(currentClient);
                    --i;
                }
            }
        }
    }
}

std::string Server::getHostname() {
    char hostname[256];
    if (gethostname(hostname, sizeof(hostname)) == -1) {
        throw std::runtime_error("Failed to get hostname: " + std::string(strerror(errno)));
    }
    return std::string(hostname);
}

Channel *Server::getChannelByName(const std::string &name) {
    std::map<std::string, Channel *>::iterator it = _channels.find(name);
    if (it != _channels.end()) {
        return it->second;
    }
    return NULL;
}

void Server::addChannel(Channel *channel) { _channels[channel->getName()] = channel; }

void Server::removeChannel(const std::string &name) {
    std::map<std::string, Channel *>::iterator it = _channels.find(name);
    if (it != _channels.end()) {
        Channel                           *channel = it->second;
        std::map<Client *, std::set<char> > members = channel->getMembers();
        for (std::map<Client *, std::set<char> >::iterator it2 = members.begin();
             it2 != members.end(); ++it2) {
            Client *member = it2->first;
            member->removeChannel(name);
        }
        delete it->second;
        _channels.erase(it);
    }
}

Client *Server::getClientByName(std::string client) {
    for (size_t i = 0; i < _clients.size(); ++i) {
        if (_clients[i]->getNickname() == client) {
            return _clients[i];
        }
    }
    return NULL;
}
