#include "../../includes/CommandHandler.h"
#include "../../includes/Server.h"
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

// TODO - Command - KICK
void CommandHandler::handleKick(Client *client, const std::vector<std::string> &input) {

    // try{

    // }
    std::cout << client->getNickname() << " called KICK" << std::endl;
    (void)input;
    std::vector<std::string> args = splitArgs(input[2]);

    Server *server = client->getServer();
    (void)server;
    Client *target = server->getClientByName(args[1]);
    (void)target;
    Channel *channel = server->getChannelByName(args[0]);

    //: userAdmin!~admin@host KICK #mon-canal user42 :Trop de spam !
    if (!channel)
        throw Error::IRCError(ERR_NOSUCHCHANNEL(client->getNickname(), args[0]).c_str());
    // ERR_NOSUCHCHANNEL
    if (!channel->isMember(client))
        throw Error::IRCError(ERR_NOTONCHANNEL(client->getNickname()).c_str());
    // ERR_NOTONCHANNEL
    if (!channel->isMember(target))
        throw Error::IRCError(ERR_USERNOTINCHANNEL(client->getNickname(), args[0]).c_str());
    // ERR_USERNOTINCHANNEL
    if (!channel->isOperator(client))
        throw Error::IRCError(ERR_CHANOPRIVSNEEDED(client->getNickname(),args[0]).c_str());

    target->sendMessage(std::string(":" + client->getUsername() + "!" + client->getNickname() +
                                    "@" + client->getHostname() + " KICK ") +
                        channel->getName() + " " + target->getNickname() + " :" + input[3] +
                        "\r\n");

    std::cout << "[" << client->getNickname() << "] Kicked [" << target->getNickname() << "] from ["
              << channel->getName() << "]" << std::endl;
}