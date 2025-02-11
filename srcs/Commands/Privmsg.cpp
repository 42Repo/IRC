#include "../../includes/CommandHandler.h"
#include "../../includes/Error.hpp"
#include "../../includes/Server.h"
#include <exception>

static void sendPrivUserMsg(Server *server, Client *send, std::string receive,
                            const std::string &message) {
    std::vector<Client *> clients = server->getClients();
    for (size_t i = 0; i < clients.size(); ++i) {
        if (clients[i]->getNickname() == receive) {
            std::cout << "message envoyé a " << clients[i]->getNickname() << std::endl;
            clients[i]->sendMessage(":" + send->getNickname() + " PRIVMSG " +
                                    clients[i]->getNickname() + " :" + message + "\n\r");
            return;
        }
    }
    throw Error::IRCError(ERR_NOSUCHNICK(receive).c_str());
}

static void sendPrivChannelMsg(Server *server, Client *send, std::string channelName,
                               const std::string &message) {
    std::cout << send->getNickname() << " sent [" << message << "] to " << channelName << std::endl;
    Channel *channel = server->getChannelByName(channelName);
    if (!channel)
        throw Error::IRCError(ERR_NOSUCHCHANNEL(send->getNickname(), channelName).c_str());
    std::map<Client *, std::set<char> > clients = channel->getMembers();
    for (std::map<Client *, std::set<char> >::iterator it = clients.begin(); it != clients.end();
         ++it) {
        if (it->first->getNickname() != send->getNickname())
            it->first->sendMessage(":" + send->getNickname() + " PRIVMSG " + channelName + " :" +
                                   message + "\n\r");
    }
}

// TODO - Command - PRIVMSG
void CommandHandler::handlePrivmsg(Client *client, const std::vector<std::string> &input) {

    std::cout << client->getNickname() << " called PRIVATEMSG" << std::endl;
    std::cout << input[2] << std::endl;
        if (input[2].size() == 0)
            throw Error::IRCError(ERR_NORECIPIENT("PRIVMSG").c_str());
        if (input[3].size() == 0)
            throw Error::IRCError(ERR_NOTEXTTOSEND(client->getNickname()).c_str());
        if (input[2][0] == '#')
            sendPrivChannelMsg(_server, client, input[2], input[3]);
        else
            sendPrivUserMsg(_server, client, input[2], input[3]);
}
