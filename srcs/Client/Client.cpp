#include "../../includes/Client.h"
#include "../../includes/Server.h"
#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <iostream>
#include <netdb.h>
#include <sstream>
#include <stdexcept>
#include <string.h>
#include <unistd.h>
#include <vector>

Client::Client(int fd, Server *server)
    : _fd(fd),
      _server(server),
      _nickname("*"),
      _username(""),
      _realname(""),
      _hostname(""),
      _isAuthenticated(false),
      _messageBuffer(""),
      _isOperator(false),
      _channels(),
      _status(""),
      _isRegistered(false) {

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

int Client::getFd() const { return _fd; }

std::string Client::getNickname() const { return _nickname; }

std::string Client::getUsername() const { return _username; }

std::string Client::getRealname() const { return _realname; }

std::string Client::getHostname() const { return _hostname; }

void Client::sendMessage(const std::string &msg) {
    ssize_t bytesSent = send(this->_fd, msg.c_str(), msg.length(), 0);
    if (bytesSent == -1) {
        // Handle error (e.g., log it, close the connection)
        // TODO: Implement error handling
    }
}

void Client::sendNumericReply(const char *numericStr, const std::string params) {
    std::ostringstream replyStream;
    replyStream << ":" << _server->getHostname();
    // << " " << _nickname
    // for (size_t i = 0; i < params.size(); ++i) {
    //     replyStream << " " << params[i];
    // }
    (void)numericStr;
    replyStream << " " << params;
    replyStream << "\r\n";

    sendMessage(replyStream.str());
}

void Client::appendToMessageBuffer(const std::string &data) { _messageBuffer += data; }

std::string Client::getMessageBuffer() const { return _messageBuffer; }

void Client::clearMessageBuffer() { _messageBuffer.clear(); }

std::map<std::string, Channel *> Client::getChannels() const { return _channels; }

void Client::removeFromMessageBuffer(size_t length) {
    if (length >= _messageBuffer.length()) {
        _messageBuffer.clear();
    } else {
        _messageBuffer = _messageBuffer.substr(length);
    }
}

void Client::joinChannel(Channel *channel) { _channels[channel->getName()] = channel; }

void Client::leaveChannel(Channel *channel) {
    if (_channels.find(channel->getName()) != _channels.end()) {
        _channels.erase(channel->getName());
    }
}

// void Client::addMode(char mode) {
//     if (_userModes.find(mode) == std::string::npos) {
//         _userModes += mode;
//     }
// }

// void Client::removeMode(char mode) {
//     size_t pos = _userModes.find(mode);
//     if (pos != std::string::npos) {
//         _userModes.erase(pos, 1);
//     }
// }

Server *Client::getServer() { return _server; }
