#include "../../includes/Client.h"
#include <fcntl.h>
#include <stdexcept>
#include <string.h>
#include <unistd.h>
#include <errno.h>


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
    setNonBlocking(_fd);
}

Client::~Client() {
	close(_fd);
}

int Client::getFd(){return _fd;}