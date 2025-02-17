#include <CommandHandler.h>
#include <Server.h>
#include <iostream>
#include <ostream>
#include <vector>

static std::vector<std::string> splitArgs(const std::string &str) {
    std::vector<std::string> tokens;
    std::stringstream        ss(str);
    std::string              token;

    while (std::getline(ss, token, ' ')) {
        tokens.push_back(token);
    }
    return tokens;
}

static void sendChannelKickMsg(Server *server, Client *send, std::string channelName,
                               const std::string &message) {
    std::cout << send->getNickname() << " sent [" << message << "] to " << channelName << std::endl;
    Channel *channel = server->getChannelByName(channelName);
    if (!channel)
        throw Error::IRCError(ERR_NOSUCHCHANNEL(send->getNickname(), channelName).c_str());
    std::map<Client *, std::set<char> > clients = channel->getMembers();
    for (std::map<Client *, std::set<char> >::iterator it = clients.begin(); it != clients.end();
         ++it) {
        it->first->sendMessage(std::string(":" + send->getUsername() + "!" + send->getNickname() +
                                           "@" + send->getHostname() + " KICK ") +
                               channel->getName() + " " + message);
    }
}

void CommandHandler::handleKick(Client *client, const std::vector<std::string> &input) {

    std::cout << client->getNickname() << " called KICK" << std::endl;
    std::vector<std::string> args = splitArgs(input[2]);

    Server  *server = client->getServer();
    Client  *target = server->getClientByName(args[1]);
    Channel *channel = server->getChannelByName(args[0]);

    if (!channel)
        throw Error::IRCError(ERR_NOSUCHCHANNEL(client->getNickname(), args[0]).c_str());
    if (!channel->isMember(client))
        throw Error::IRCError(ERR_NOTONCHANNEL(client->getNickname(), args[0]).c_str());
    if (!channel->isMember(target))
        throw Error::IRCError(ERR_USERNOTINCHANNEL(client->getNickname(), args[0]).c_str());
    if (!channel->isOperator(client))
        throw Error::IRCError(ERR_CHANOPRIVSNEEDED(client->getNickname(), args[0]).c_str());

    channel->removeMember(target);
    target->removeChannel(channel->getName());

    sendChannelKickMsg(_server, client, args[0], target->getNickname() + " :" + input[3] + "\r\n");
    target->sendMessage(std::string(":" + client->getUsername() + "!" + client->getNickname() +
                                    "@" + client->getHostname() + " KICK ") +
                        channel->getName() + " " + target->getNickname() + " :" + input[3] +
                        "\r\n");
    std::cout << "[" << client->getNickname() << "] Kicked [" << target->getNickname() << "] from ["
              << channel->getName() << "]" << std::endl;
}
