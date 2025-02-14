#include "../../includes/CommandHandler.h"
#include "../../includes/Server.h"
#include <ostream>
#include <sys/select.h>
#include <sys/time.h>

// TODO - Add RPL 333

void CommandHandler::handleTopic(Client *client, const std::vector<std::string> &input) {
    std::cout << client->getNickname() << " called TOPIC" << std::endl;
    struct timeval tv;
    gettimeofday(&tv, NULL);
    if (input[2].length() < 1)
        throw Error::IRCError(ERR_NEEDMOREPARAMS("INVITE").c_str());
    Server  *server = client->getServer();
    Channel *channel = server->getChannelByName(input[2]);
    // std::cout << "[" << input[3] << "]" << std::endl;
    if (!channel)
        throw Error::IRCError(ERR_NOSUCHCHANNEL(client->getNickname(), input[1]).c_str());
    if (!channel->isMember(client))
        throw Error::IRCError(ERR_NOTONCHANNEL(client->getNickname(), channel->getName()).c_str());

    if (input[3].length() > 1) {
        if (!channel->isOperator(client))
            throw Error::IRCError(
                ERR_CHANOPRIVSNEEDED(client->getNickname(), channel->getName()).c_str());
        channel->setTimestamp(tv.tv_sec);
        channel->setTopic(input[3]);
        client->sendMessage(std::string(":") + client->getNickname() + "!" + client->getUsername() +
                            "@" + client->getHostname() + " TOPIC " + input[2] + " :" +
                            channel->getTopic() + "\r\n");
    } else {
        client->sendNumericReply(RPL_TOPIC(client->getNickname(), input[2], channel->getTopic()));

        client->sendNumericReply(
            RPL_TOPICTIME(client->getNickname(), input[2], channel->getTopic()));
    }
    // 333	Infos sur la dernière modification du sujet (RPL_TOPICWHOTIME).
}

// :irc.example.com 332 user42 #gaming :Bienvenue sur le canal de discussion !

//: user42!~user@host TOPIC #gaming :Nouvelle discussion sur les jeux rétro !
/*
 */
