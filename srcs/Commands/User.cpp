#include <CommandHandler.h>

void CommandHandler::handleUser(Client *client, const std::vector<std::string> &input) {
    std::istringstream iss(input[2] + " " + input[3]);
    std::string        user, mode, unused, realname;

    iss >> user >> mode >> unused;
    std::getline(iss >> std::ws, realname);

    if (realname.empty() || user.empty()) {
        client->sendNumericReply(ERR_NEEDMOREPARAMS("USER"));
        return;
    }

    if (client->getIsRegistered()) {
        client->sendNumericReply(ERR_ALREADYREGISTRED(client->getNickname()));
        return;
    }

    client->setUsername(user);
    client->setRealname(realname);

    if (!client->getNickname().empty() && client->getNickname() != "*" &&
        !client->getIsRegistered() && client->getIsAuthenticaded())
        welcomeMsg(client);
}
