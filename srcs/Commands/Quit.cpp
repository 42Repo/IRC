#include "../../includes/CommandHandler.h"
#include "../../includes/Server.h"
#include <vector>

static int isSharingChannel(Client *client, Client *target) {
    std::map<std::string, Channel *> clientChannels = client->getChannels();
    std::map<std::string, Channel *> targetChannels = target->getChannels();

    // for (std::map<std::string, Channel *>::iterator it = clientChannels.begin();
    //      it != clientChannels.end(); ++it) {
    //     for (std::map<std::string, Channel *>::iterator it1 = targetChannels.begin();
    //          it1 != targetChannels.end(); ++it1) {
    //         if (it == it1)
    //             return 1;
    //     }
    // }
    for (std::map<std::string, Channel *>::iterator it = clientChannels.begin();
         it != clientChannels.end(); ++it) {
        if (targetChannels[it->first] == it->second)
            return 1;
    }
    return 0;
}

void CommandHandler::handleQuit(Client *client, const std::vector<std::string> &input) {
    std::cout << client->getNickname() << " called QUIT" << std::endl;
    Server               *server = client->getServer();
    std::vector<Client *> clients = server->getClients();

    for (std::vector<Client *>::iterator it = clients.begin(); it != clients.end(); ++it) {
        if ((*it) != client && isSharingChannel(client, *it)) {
            (*it)->sendMessage(":" + client->getNickname() + "!" + client->getUsername() + "@" +
                               client->getHostname() + " QUIT :" + input[3] + "\n\r");
        }
    }

    std::map<std::string, Channel *> channels = server->getChannels();
    for (std::map<std::string, Channel *>::iterator it = channels.begin(); it != channels.end();
         ++it) {
        if (it->second->isMember(client)) {
            it->second->removeMember(client);
            client->removeChannel(it->first);
        }
    }
    server->removeClient(client);
}

//: UserNick!~ident@host QUIT :Quit message
