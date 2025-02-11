#include "../../includes/Error.hpp"
#include "../../includes/Server.h"
#include <sstream>
Error::Error(std::string errMessage, Client *client) {
    std::ostringstream replyStream;
    _server = client->getServer();
    replyStream << ":" << _server->getHostname();
    replyStream << " " << errMessage;
    replyStream << "\r\n";

    client->sendMessage(replyStream.str());
}

Error::IRCError::IRCError(const char *msg) {
    _msg = new char[strlen(msg) + 1]; // Allocation dynamique
    strcpy(_msg, msg);
};
Error::IRCError::~IRCError() throw() { delete[] _msg; };

const char *Error::IRCError::what() const throw() { return _msg; };