#include "../../includes/Error.hpp"
#include <sstream>
#include "../../includes/Server.h"


Error::Error(std::string errMessage, Client *client)
{
	std::ostringstream replyStream;
	_server = client->getServer();
    replyStream << ":" << _server->getHostname();
    // << " " << _nickname
    // for (size_t i = 0; i < params.size(); ++i) {
    //     replyStream << " " << params[i];
    // }
    replyStream << " " << errMessage;
    replyStream << "\r\n";

    client->sendMessage(replyStream.str());
}