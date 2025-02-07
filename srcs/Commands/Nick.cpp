#include "../../includes/CommandHandler.h"
#include "../../includes/Server.h"

static bool isValidNickname(const std::string &nick) {
    if (nick.length() < 1 || nick.length() > 9) {
        return false;
    }
    if (!std::isalpha(nick[0]) && nick[0] != '[' && nick[0] != '{' && nick[0] != '\\' &&
        nick[0] != ']' && nick[0] != '}' && nick[0] != '|' && nick[0] != '_') {
        return false;
    }
    for (size_t i = 1; i < nick.length(); ++i) {
        if (!std::isalnum(nick[i]) && nick[i] != '-' && nick[i] != '[' && nick[i] != ']' &&
            nick[i] != '{' && nick[i] != '}' && nick[i] != '\\' && nick[i] != '|' &&
            nick[i] != '_') {
            return false;
        }
    }
    return true;
}

// TODO - Command - NICK
void CommandHandler::handleNick(Client *client, const std::vector<std::string> &input) {

    if (!client->getIsAuthenticaded()) {
        client->sendNumericReply("451", ERR_NOTREGISTERED(client->getNickname()));
        return;
    }

    if (input[2].length() == 0) {
        client->sendNumericReply("431", ERR_NONICKNAMEGIVEN);
        return;
    }

    if (!isValidNickname(input[2])) {
        client->sendNumericReply("432", ERR_ERRONEUSNICKNAME(input[2]));
        return;
    }

    // Check if the nickname is already used
    std::vector<Client *> clients = _server->getClients();
    for (size_t i = 0; i < clients.size(); ++i) {
        if (clients[i] != client && clients[i]->getNickname() == input[2]) {
            client->sendNumericReply("433", ERR_NICKNAMEINUSE(input[2]));
            return;
        }
    }

    std::string oldNick = client->getNickname();
    client->setNickname(input[2]);

    if (client->getIsRegistered()) {
        std::string nickChangeMessage = ":" + oldNick + "!" + client->getUsername() + "@" +
                                        client->getHostname() + " NICK :" + input[2] + "\r\n";

        for (size_t i = 0; i < clients.size(); ++i) {
            clients[i]->sendMessage(nickChangeMessage);
        }
    }

    if (!client->getUsername().empty() && client->getNickname() != "*" &&
        !client->getIsRegistered() && client->getIsAuthenticaded())
        welcomeMsg(client);

    std::cout << "[" << client->getHostname() << "] Nickname set to " << input[2] << std::endl;
}
