#include "../../includes/CommandHandler.h"
#include "../../includes/Server.h"

// TODO - Command - PASS
void CommandHandler::handlePass(Client *client, const std::vector<std::string> &input) {

    if (input[2].length() == 0) {
        client->sendNumericReply("461", ERR_NEEDMOREPARAMS("PASS"));
        return;
    }
    if (client->getIsAuthenticaded()) {
        client->sendNumericReply("462", ERR_ALREADYREGISTRED(client->getNickname()));
        return;
    }
    if (_server->getPassword() != input[2]) {
        client->sendNumericReply("464", ERR_PASSWDMISMATCH(client->getNickname()));
        return;
    }
    client->setAuthenticaded(true);
    std::cout << client->getUsername() << " identified" << std::endl;
}
