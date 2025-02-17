#include <CommandHandler.h>
#include <Server.h>

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

void CommandHandler::handleNick(Client *client, const std::vector<std::string> &input) {

    if (!client->getIsAuthenticaded())
        throw Error::IRCError(ERR_NOTREGISTERED(client->getNickname()).c_str());

    if (input[2].length() == 0)
        throw Error::IRCError(ERR_NONICKNAMEGIVEN.c_str());

    if (!isValidNickname(input[2]))
        throw Error::IRCError(ERR_ERRONEUSNICKNAME(input[2]).c_str());

    // Check if the nickname is already used
    std::vector<Client *> clients = _server->getClients();
    for (size_t i = 0; i < clients.size(); ++i) {
        if (clients[i] != client && clients[i]->getNickname() == input[2])
            throw Error::IRCError(ERR_NICKNAMEINUSE(input[2]).c_str());
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
