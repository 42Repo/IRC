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

bool isValidChannelName(const std::string &name) {
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

std::string getMemberList(Channel *channel) {
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
    std::vector<std::string> channels = splitString(input[2], ',');
    std::vector<std::string> passwords;
    if (input.size() > 3) {
        passwords = splitString(input[3], ',');
    }

    if (channels.empty()) {
        client->sendNumericReply("461", ERR_NEEDMOREPARAMS("JOIN"));
        return;
    }

    for (size_t i = 0; i < channels.size(); ++i) {
        std::string channelName = channels[i];
        std::string password = (i < passwords.size()) ? passwords[i] : "";

        if (!isValidChannelName(channelName)) {
            client->sendNumericReply("476", ERR_BADCHANMASK(channelName));
            continue;
        }

        Channel *channel = _server->getChannelByName(channelName);
        if (!channel) {
            channel = new Channel(channelName, client);
            _server->addChannel(channel);
            channel->addMember(client);
            client->joinChannel(channel);

            std::string joinMessage = ":" + client->getNickname() + "!" + client->getUsername() +
                                      "@" + client->getHostname() + " JOIN :" + channelName +
                                      "\r\n";

            std::map<Client *, std::set<char> > members = channel->getMembers();
            for (std::map<Client *, std::set<char> >::iterator it = members.begin();
                 it != members.end(); ++it) {
                Client *member = it->first;
                member->sendMessage(joinMessage);
            }

            channel->addChannelMode('t');
            channel->addUserMode(client, 'o');
            std::string modeMessage =
                ":" + _server->getHostname() + " MODE " + channelName + " +t " + client->getNickname() + "\r\n";
            client->sendMessage(modeMessage);

        } else {
            if (channel->hasChannelMode('i')) {
                client->sendNumericReply("473", ERR_INVITEONLYCHAN(channelName));
                continue;
            }
            if (channel->getPassword() != "" && channel->getPassword() != password) {
                client->sendNumericReply("475", ERR_BADCHANNELKEY(channelName));
                continue;
            }

            if (channel->getUserLimit() > 0 &&
                channel->getMembers().size() >= (size_t)channel->getUserLimit()) {
                client->sendNumericReply("471", ERR_CHANNELISFULL(channelName));
                continue;
            }

            channel->addMember(client);
            client->joinChannel(channel);

            std::string joinMessage = ":" + client->getNickname() + "!" + client->getUsername() +
                                      "@" + client->getHostname() + " JOIN :" + channelName +
                                      "\r\n";
            std::map<Client *, std::set<char> > members = channel->getMembers();
            for (std::map<Client *, std::set<char> >::iterator it = members.begin();
                 it != members.end(); ++it) {
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
}
