#include "../../includes/CommandHandler.h"
#include "../../includes/Server.h"

static std::vector<std::string> splitString(const std::string &str, char delimiter) {
    std::vector<std::string> tokens;
    std::stringstream        ss(str);
    std::string              token;

    while (std::getline(ss, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

static bool isValidChannelName(const std::string &name) {
    if (name.empty() || name.size() > 50) {
        return false;
    }
    if (name[0] != '#' && name[0] != '&') {
        return false;
    }
    for (size_t i = 0; i < name.size(); ++i) {
        if (name[i] == ' ' || name[i] == 7 || name[i] == ',')
            return false;
    }
    return true;
}

static std::string getMemberList(Channel *channel) {
    std::string                        memberList;
    std::map<Client *, std::set<char> > members = channel->getMembers();
    for (std::map<Client *, std::set<char> >::iterator it = members.begin(); it != members.end();
         ++it) {
        Client *member = it->first;
        if (channel->hasUserMode(member, 'o')) {
            memberList += "@";
        }
        memberList += member->getNickname() + " ";
    }
    return memberList;
}

void CommandHandler::handleJoin(Client *client, const std::vector<std::string> &input) {
    std::vector<std::string> params = splitString(input[2], ' ');
    std::string              channelName = params[0];
    std::string              password = (params.size() > 1) ? params[1] : "";

    if (channelName.empty()) {
        throw Error::IRCError(ERR_NEEDMOREPARAMS("JOIN").c_str());
    }

    if (!isValidChannelName(channelName)) {
        throw Error::IRCError(ERR_BADCHANMASK(channelName).c_str());
    }

    Channel *channel = _server->getChannelByName(channelName);
    if (!channel) {
        channel = new Channel(channelName, client);
        _server->addChannel(channel);
        channel->addMember(client);
        client->joinChannel(channel);

        std::string joinMessage = ":" + client->getNickname() + "!" + client->getUsername() + "@" +
                                  client->getHostname() + " JOIN :" + channelName + "\r\n";

        std::map<Client *, std::set<char> > members = channel->getMembers();
        for (std::map<Client *, std::set<char> >::iterator it = members.begin(); it != members.end();
             ++it) {
            Client *member = it->first;
            member->sendMessage(joinMessage);
        }

        channel->addChannelMode('t');
        channel->addUserMode(client, 'o');
        std::string modeMessage = ":" + _server->getHostname() + " MODE " + channelName + " +t " +
                                  client->getNickname() + "\r\n";
        client->sendMessage(modeMessage);

    } else {
        if (channel->hasChannelMode('i')) {
            throw Error::IRCError(ERR_INVITEONLYCHAN(channelName).c_str());
        }
        if (channel->hasChannelMode('k') && channel->getPassword() != password) {
            throw Error::IRCError(ERR_BADCHANNELKEY(channelName).c_str());
        }

        if (channel->getUserLimit() > 0 &&
            channel->getMembers().size() >= static_cast<size_t>(channel->getUserLimit())) {
            throw Error::IRCError(ERR_CHANNELISFULL(channelName).c_str());
        }

        channel->addMember(client);
        client->joinChannel(channel);

        std::string joinMessage = ":" + client->getNickname() + "!" + client->getUsername() + "@" +
                                  client->getHostname() + " JOIN :" + channelName + "\r\n";
        std::map<Client *, std::set<char> > members = channel->getMembers();
        for (std::map<Client *, std::set<char> >::iterator it = members.begin(); it != members.end();
             ++it) {
            Client *member = it->first;
            member->sendMessage(joinMessage);
        }
    }

    // Envoyer RPL_NAMREPLY et RPL_ENDOFNAMES
    std::string memberList = getMemberList(channel);
    client->sendMessage(":" + _server->getHostname() + " 353 " + client->getNickname() + " = " +
                        channelName + " :" + memberList + "\r\n");
    client->sendMessage(":" + _server->getHostname() + " 366 " + client->getNickname() + " " +
                        channelName + " :End of /NAMES list\r\n");
}
