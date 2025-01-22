#include "../../includes/Client.h"
#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <netdb.h>
#include <stdexcept>
#include <string.h>
#include <unistd.h>

Client::Client(int fd, Server *server)
    : _fd(fd),
      _server(server),
      _nickname(""),
      _username(""),
      _realname(""),
      _hostname(""),
      _isAuthenticated(false),
      _messageBuffer(""),
      _isOperator(false),
      _channels(),
      _status("") {

    struct sockaddr_in clientAddress;
    socklen_t          clientAddressLength = sizeof(clientAddress);
    if (getpeername(_fd, (struct sockaddr *)&clientAddress, &clientAddressLength) == 0) {
        char hostname[NI_MAXHOST];
        if (getnameinfo((struct sockaddr *)&clientAddress, clientAddressLength, hostname,
                        NI_MAXHOST, NULL, 0, NI_NAMEREQD) == 0) {
            _hostname = hostname;
        } else {
            _hostname = inet_ntoa(clientAddress.sin_addr);
        }
    } else {
        _hostname = "unknown";
    }
}

Client::~Client() { close(_fd); }

int Client::getFd() { return _fd; }

std::string Client::getNickname() { return _nickname; }

std::string Client::getUsername() { return _username; }

std::string Client::getRealname() { return _realname; }

std::string Client::getHostname() { return _hostname; }

void Client::sendMessage(const std::string &msg) {
    ssize_t bytesSent = send(this->_fd, msg.c_str(), msg.length(), 0);
    if (bytesSent == -1) {
        // Handle error (e.g., log it, close the connection)
        // TODO: Implement error handling
    }
}
