#include <Error.h>
#include <Server.h>
#include <sstream>

Error::Error(std::string errMessage, Client *client) {
    std::ostringstream replyStream;
    _server = client->getServer();
    replyStream << ":" << _server->getHostname();
    replyStream << " " << errMessage;
    replyStream << "\r\n";

    client->sendMessage(replyStream.str());
}

Error::~Error() {}

Error::IRCError::IRCError(const char *msg) {
    _msg = new char[strlen(msg) + 1];
    strcpy(_msg, msg);
}

Error::IRCError::~IRCError() throw() { delete[] _msg; }

const char *Error::IRCError::what() const throw() { return _msg; }
