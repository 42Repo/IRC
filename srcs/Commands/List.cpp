#include <CommandHandler.h>
#include <Server.h>
#include <sstream>
#include <string>

static std::string intToStr(size_t num) {
    std::ostringstream oss;
    oss << num;
    return oss.str();
}

void CommandHandler::handleList(Client *client, const std::vector<std::string> &input) {

    Server                          *server = client->getServer();
    std::map<std::string, Channel *> channels = server->getChannels();

    (void)input;

    client->sendNumericReply(RPL_LISTSTART(client->getNickname()));
    for (std::map<std::string, Channel *>::iterator it = channels.begin(); it != channels.end();
         ++it) {
        client->sendNumericReply(RPL_LIST(client->getNickname(), it->second->getName(),
                                          intToStr(it->second->getMembers().size()),
                                          it->second->getTopic()));
    }
    client->sendNumericReply(RPL_LISTEND(client->getNickname()));
}
