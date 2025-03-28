#include <CommandHandler.h>
#include <Server.h>

static std::vector<std::string> splitArgs(const std::string &str) {
    std::vector<std::string> tokens;
    std::stringstream        ss(str);
    std::string              token;

    while (std::getline(ss, token, ' ')) {
        tokens.push_back(token);
    }
    return tokens;
}

void CommandHandler::handleInvite(Client *client, const std::vector<std::string> &input) {
    (void)input;
    std::vector<std::string> args = splitArgs(input[2]);

    Server  *server = client->getServer();
    Client  *target = server->getClientByName(args[0]);
    Channel *channel = server->getChannelByName(args[1]);
    if (args.size() < 2)
        throw Error::IRCError(ERR_NEEDMOREPARAMS("INVITE").c_str());

    if (!target) {
        client->sendNumericReply(ERR_NOSUCHNICK(client->getNickname(), args[0]));
        return;
    }
    
    if (!channel) {
        client->sendNumericReply(ERR_NOSUCHCHANNEL(client->getNickname(), args[1]));
        return;
    }
    if(channel->isMember(target) || client == target) {
        client->sendNumericReply(ERR_USERONCHANNEL(client->getNickname(), args[0], args[1]));
        return;
    }
    
    channel->addInvitedUser(args[0]);
    client->sendNumericReply(RPL_INVITING(client->getNickname(), args[1], args[0]));

    target->sendMessage(std::string(":") + client->getUsername() + "!" + client->getNickname() +
                        "@" + client->getHostname() + " INVITE " + target->getNickname() + " :" +
                        args[1] + "\r\n");
}
