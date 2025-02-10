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
    std::cout << client->getNickname() << " called KICK" << std::endl;
    (void)input;
    std::vector<std::string> args = splitArgs(input[2]);

    Server *server = client->getServer();
    (void)server;
    Client *target = server->getClientByName(args[1]);
    (void)target;
    Channel *channel = server->getChannelByName(args[0]);
    if (channel)
        std::cout << "target = " << target->getNickname() << "\nChannel = " << channel->getName()
                  << std::endl;
    else
        std::cout << "Channel not found" << std::endl;
    (void)channel;

    std::cout << "\ncmd channel : [" << args[0] << "]\ncmd target : [" << args[1]
              << "]\nKick message : [" << input[3] << "]" << std::endl;


    target->sendMessage(std::string(":" + client->getUsername() + "!" + client->getNickname()+ "@" + client->getHostname() + " KICK ") + channel->getName() + " " + target->getNickname() + " :" + input[3] + "\r\n");

    //:userAdmin!~admin@host KICK #mon-canal user42 :Trop de spam !

    /*
        if(!channel)
            ERR_NOSUCHCHANNEL
        if(not on channel)
            ERR_NOTONCHANNEL
        if(target not on channel)
            ERR_USERNOTINCHANNEL
        if(!user op)
            ERR_CHANOPRIVSNEEDED
    */
    std::cout << "[" << client->getNickname() << "] Kicked [" << target->getNickname() << "] from [" << channel->getName()<< "]" << std::endl;
}