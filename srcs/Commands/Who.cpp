#include <CommandHandler.h>
#include <Error.h>
#include <Server.h>
#include <fnmatch.h>

static bool matchesMask(const std::string &str, const std::string &mask) {
    return fnmatch(mask.c_str(), str.c_str(), 0) == 0;
}

static std::vector<std::string> splitString(const std::string &str, char delimiter) {
    std::vector<std::string> tokens;
    std::stringstream        ss(str);
    std::string              token;

    while (std::getline(ss, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

static std::string buildFlags(Client *client, Channel *channel) {
    std::string flags = "H";
    if (channel && channel->isOperator(client)) {
        flags += "@";
    }
    return flags;
}

static void sendWhoReply(Client *sender, Client *target, const std::string &channelName,
                         const std::string &flags) {
    sender->sendNumericReply(RPL_WHOREPLY(sender->getNickname(), channelName, target->getUsername(),
                                          target->getHostname(), sender->getServer()->getHostname(),
                                          target->getNickname(), flags, "0",
                                          target->getRealname()));
}

static void processWhoAll(Client *client, Server *server) {
    std::vector<Client *> clients = server->getClients();
    for (size_t i = 0; i < clients.size(); ++i) {
        std::string flags = buildFlags(clients[i], NULL);
        sendWhoReply(client, clients[i], "*", flags);
    }
}

static void processWhoChannel(Client *client, const std::string &channelName, bool onlyOperators,
                              Server *server) {
    Channel *channel = server->getChannelByName(channelName);
    if (!channel) {
        throw Error::IRCError(ERR_NOSUCHCHANNEL(client->getNickname(), channelName).c_str());
    }

    if (!channel->isMember(client) && channel->hasChannelMode('s')) {
        client->sendNumericReply(RPL_ENDOFWHO(client->getNickname(), channelName));
        return;
    }

    std::map<Client *, std::set<char> > members = channel->getMembers();
    for (std::map<Client *, std::set<char> >::iterator it = members.begin(); it != members.end();
         ++it) {
        Client *member = it->first;
        if (onlyOperators && !channel->isOperator(member)) {
            continue;
        }
        std::string flags = buildFlags(member, channel);
        sendWhoReply(client, member, channel->getName(), flags);
    }
}

static void processWhoMask(Client *client, const std::string &mask,
                           Server *server) {
    std::vector<Client *> clients = server->getClients();
    for (size_t i = 0; i < clients.size(); ++i) {

        if ((matchesMask(clients[i]->getNickname(), mask) ||
             matchesMask(clients[i]->getUsername(), mask) ||
             matchesMask(clients[i]->getHostname(), mask))) {
            std::string flags = buildFlags(clients[i], NULL);
            sendWhoReply(client, clients[i], "*", flags);
        }
    }
}

void CommandHandler::handleWho(Client *client, const std::vector<std::string> &input) {
    std::vector<std::string> params = splitString(input[2], ' ');
    std::string              mask = params.empty() ? "" : params[0];
    bool                     onlyOperators = (params.size() > 1) && (params[1] == "o");

    try {
        if (mask.empty() || mask == "*" || mask == "0") {
            processWhoAll(client, _server);
        } else if (mask[0] == '#' || mask[0] == '&') {
            processWhoChannel(client, mask, onlyOperators, _server);
        } else {
            processWhoMask(client, mask, _server);
        }
    } catch (const std::exception &e) {
        client->sendNumericReply(
            RPL_ENDOFWHO(client->getNickname(), mask));
        throw Error::IRCError(e.what());
    }

    client->sendNumericReply(RPL_ENDOFWHO(client->getNickname(), mask));
}
