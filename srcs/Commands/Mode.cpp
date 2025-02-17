#include <CommandHandler.h>
#include <Error.h>
#include <Server.h>
#include <sstream>

static void parseModeInput(const std::string &input, std::string &target, std::string &modeString,
                           std::string &argument) {
    std::istringstream iss(input);
    std::string        temp;

    iss >> target;
    iss >> modeString;

    std::getline(iss >> std::ws, argument);
}

static void sendModeMessage(Channel *channel, const std::string &message) {
    std::map<Client *, std::set<char> > members = channel->getMembers();
    for (std::map<Client *, std::set<char> >::iterator it = members.begin(); it != members.end();
         ++it) {
        it->first->sendMessage(message);
    }
}

static void handleModeInviteOnly(Client *client, Channel *channel, bool adding) {
    if (!channel->isOperator(client)) {
        throw Error::IRCError(
            ERR_CHANOPRIVSNEEDED(client->getNickname(), channel->getName()).c_str());
    }
    if (adding) {
        if (channel->hasChannelMode('i')) {
            return;
        }
        channel->addChannelMode('i');
    } else {
        if (!channel->hasChannelMode('i')) {
            return;
        }
        channel->removeChannelMode('i');
    }
    std::string modeMessage = ":" + client->getNickname() + "!" + client->getUsername() + "@" +
                              client->getHostname() + " MODE " + channel->getName() + " " +
                              (adding ? "+i" : "-i") + "\r\n";
    sendModeMessage(channel, modeMessage);
}

static void handleModeTopicRestricted(Client *client, Channel *channel, bool adding) {
    if (!channel->isOperator(client)) {
        throw Error::IRCError(
            ERR_CHANOPRIVSNEEDED(client->getNickname(), channel->getName()).c_str());
    }

    if (adding) {
        if (channel->hasChannelMode('t')) {
            return;
        }
        std::cout << "Adding topic restricted mode" << std::endl;
        channel->addChannelMode('t');
    } else {
        if (!channel->hasChannelMode('t')) {
            return;
        }
        channel->removeChannelMode('t');
    }

    std::string modeMessage = ":" + client->getNickname() + "!" + client->getUsername() + "@" +
                              client->getHostname() + " MODE " + channel->getName() + " " +
                              (adding ? "+t" : "-t") + "\r\n";
    sendModeMessage(channel, modeMessage);
}

static void handleModeKey(Client *client, Channel *channel, bool adding, std::string &argument) {
    if (!channel->isOperator(client)) {
        throw Error::IRCError(
            ERR_CHANOPRIVSNEEDED(client->getNickname(), channel->getName()).c_str());
    }

    std::string first_arg = argument.substr(0, argument.find(' '));
    argument = argument.substr(argument.find(' ') + 1);

    if (adding) {
        if (argument.empty())
            throw Error::IRCError(ERR_NEEDMOREPARAMS("MODE").c_str());
        channel->addChannelMode('k');
        channel->setPassword(argument);
    } else {
        channel->removeChannelMode('k');
        channel->setPassword("");
    }
    std::string modeMessage = ":" + client->getNickname() + "!" + client->getUsername() + "@" +
                              client->getHostname() + " MODE " + channel->getName() + " " +
                              (adding ? "+k" : "-k") + " " + argument + "\r\n";
    sendModeMessage(channel, modeMessage);
}

static void handleModeOperator(Client *client, Channel *channel, bool adding, std::string &argument,
                               Server *server) {

    if (!channel->isOperator(client)) {
        throw Error::IRCError(
            ERR_CHANOPRIVSNEEDED(client->getNickname(), channel->getName()).c_str());
    }

    std::string first_arg = argument.substr(0, argument.find(' '));
    argument = argument.substr(argument.find(' ') + 1);

    if (first_arg.empty()) {
        throw Error::IRCError(ERR_NEEDMOREPARAMS("MODE").c_str());
    }

    Client *targetClient = server->getClientByName(first_arg);
    if (!targetClient) {
        throw Error::IRCError(ERR_NOSUCHNICK(client->getNickname(), first_arg).c_str());
    }

    if (!channel->isMember(targetClient)) {
        throw Error::IRCError(ERR_NOTONCHANNEL(client->getNickname(), first_arg).c_str());
    }

    // TODO : peut etre ajouter un check ici
    if (adding) {
        channel->addUserMode(targetClient, 'o');
    } else {
        channel->removeUserMode(targetClient, 'o');
    }

    std::string modeMessage = ":" + client->getNickname() + "!" + client->getUsername() + "@" +
                              client->getHostname() + " MODE " + channel->getName() + " " +
                              (adding ? "+o" : "-o") + " " + first_arg + "\r\n";

    sendModeMessage(channel, modeMessage);
}

static void handleModeLimit(Client *client, Channel *channel, bool adding, std::string &argument) {
    if (!channel->isOperator(client)) {
        throw Error::IRCError(
            ERR_CHANOPRIVSNEEDED(client->getNickname(), channel->getName()).c_str());
    }

    std::string first_arg = argument.substr(0, argument.find(' '));
    argument = argument.substr(argument.find(' ') + 1);

    if (adding) {
        if (argument.empty())
            throw Error::IRCError(ERR_NEEDMOREPARAMS("MODE").c_str());
        channel->addChannelMode('l');
        std::istringstream iss(argument);
        int                limit;
        if (!(iss >> limit) || limit <= 0) {
            throw Error::IRCError(ERR_NEEDMOREPARAMS("MODE").c_str());
        }
        channel->setUserLimit(limit);
    } else {
        channel->removeChannelMode('l');
        channel->setUserLimit(0);
    }
    std::string modeMessage = ":" + client->getNickname() + "!" + client->getUsername() + "@" +
                              client->getHostname() + " MODE " + channel->getName() + " " +
                              (adding ? "+l" : "-l") + " " + argument + "\r\n";

    sendModeMessage(channel, modeMessage);
}

static void displayChannelModes(Client *client, Channel *channel) {
    std::map<Client *, std::set<char> > members = channel->getMembers();
    std::string                        userModesString;
    std::string                        serverModesString;
    userModesString = channel->getUserModes(client);
    if (userModesString.find('o') != std::string::npos) {
        userModesString.erase(userModesString.find('o'), 1);
    }
    serverModesString = channel->getChannelModes();
    if (!userModesString.empty() || !serverModesString.empty()) {
        client->sendNumericReply("324 " + client->getNickname() + " " + channel->getName() + " " +
                                 "+" + serverModesString + userModesString);
    }
}

void CommandHandler::handleMode(Client *client, const std::vector<std::string> &input) {
    if (input.size() < 3 || input[2].empty()) {
        throw Error::IRCError(ERR_NEEDMOREPARAMS("MODE").c_str());
    }
    if (!client->getIsRegistered()) {
        throw Error::IRCError(ERR_NOTREGISTERED(client->getNickname()).c_str());
    }

    std::string target;
    std::string modeString;
    std::string argument;

    parseModeInput(input[2], target, modeString, argument);

    if (target[0] == '#') {
        Channel *channel = _server->getChannelByName(target);
        if (!channel) {
            throw Error::IRCError(ERR_NOSUCHCHANNEL(client->getNickname(), target).c_str());
        }
        if (!channel->isMember(client)) {
            throw Error::IRCError(ERR_NOTONCHANNEL(client->getNickname(), target).c_str());
        }

        if (modeString.empty()) {
            displayChannelModes(client, channel);
            return;
        }

        bool adding = true;
        for (size_t i = 0; i < modeString.length(); ++i) {
            char modeChar = modeString[i];
            if (modeChar == '+') {
                adding = true;
                continue;
            } else if (modeChar == '-') {
                adding = false;
                continue;
            }
            switch (modeChar) {
            case 'i':
                handleModeInviteOnly(client, channel, adding);
                break;
            case 't':
                handleModeTopicRestricted(client, channel, adding);
                break;
            case 'k':
                handleModeKey(client, channel, adding, argument);
                break;
            case 'o':
                handleModeOperator(client, channel, adding, argument, _server);
                break;
            case 'l':
                handleModeLimit(client, channel, adding, argument);
                break;
            default:
                client->sendNumericReply(ERR_UNKNOWNMODE(modeChar, client->getNickname()));
                break;
            }
        }
    } else { // User modes (not supported)
        client->sendNumericReply(ERR_UNKNOWNMODE(modeString, client->getNickname()));
    }
}
