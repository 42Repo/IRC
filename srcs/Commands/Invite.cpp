#include "../../includes/CommandHandler.h"
#include "../../includes/Server.h"

static std::vector<std::string> splitArgs(const std::string &str) {
    std::vector<std::string> tokens;
    std::stringstream        ss(str);
    std::string              token;

    while (std::getline(ss, token, ' ')) {
        tokens.push_back(token);
    }
    return tokens;
}
//TODO - fix error messages (check the 4 errors)
//TODO - check num args

void CommandHandler::handleInvite(Client *client, const std::vector<std::string> &input) {
    std::cout << client->getNickname() << " called INVITE" << std::endl;
    (void)input;
    std::vector<std::string> args = splitArgs(input[2]);

    Server  *server = client->getServer();
    Client  *target = server->getClientByName(args[0]);
    Channel *channel = server->getChannelByName(args[1]);
    if(args.size() < 2)
        throw Error::IRCError(ERR_NEEDMOREPARAMS("INVITE").c_str());
    if (!channel || !channel->isMember(client))
        throw Error::IRCError(ERR_NOTONCHANNEL(client->getNickname(), args[1]).c_str());
    if (!channel->isOperator(client))
        throw Error::IRCError(ERR_CHANOPRIVSNEEDED(client->getNickname(), args[1]).c_str());
    if (!target)
        throw Error::IRCError(ERR_NOSUCHNICK(client->getNickname(), args[0]).c_str());
    if (channel->isMember(target))
        throw Error::IRCError(ERR_USERONCHANNEL(client->getNickname(), args[0], args[1]).c_str());
   
    channel->addInvitedUser(args[0]);
   
    client->sendNumericReply("341", RPL_INVITING(client->getNickname(),args[1], args[0]));

    target->sendMessage(std::string(":") + client->getUsername() + "!" + client->getNickname() +
                        "@" + client->getHostname() + " INVITE " + target->getNickname() + " :" +
                        args[1] + "\r\n");
}