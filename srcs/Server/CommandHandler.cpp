#include "../../includes/CommandHandler.h"
#include "../../includes/Messages.h"
#include "../../includes/Server.h"
#include <cstdlib>
#include <cstring>
#include <sstream>
#include <string>
#include <vector>

// TODO - Command - PASS
void CommandHandler::handlePass(Client *client, const std::vector<std::string> &input) {

    if (input[2].length() == 0) {
        client->sendNumericReply("461", ERR_NEEDMOREPARAMS("PASS"));
        return;
    }
    if (client->getIsAuthenticaded()) {
        client->sendNumericReply("462", ERR_ALREADYREGISTRED);
        return;
    }
    if (_server->getPassword() != input[2]) {
        client->sendNumericReply("464", ERR_PASSWDMISMATCH(client->getNickname()));
        return;
    }
    client->setAuthenticaded(true);
    std::cout << client->getUsername() << " identified" << std::endl;
}

void CommandHandler::welcomeMsg(Client *client) {
    client->setIsRegistered(true);

    client->sendNumericReply(
        "001", RPL_WELCOME(client->getNickname(), client->getUsername(), client->getHostname()));
    client->sendNumericReply("002",
                             RPL_YOURHOST(client->getNickname(), _server->getHostname(), "1.0"));
    client->sendNumericReply("003", RPL_CREATED(client->getNickname(), "DATE"));
    client->sendNumericReply(
        "004", RPL_MYINFO(client->getNickname(), _server->getHostname(), "1.0", "", ""));
}

// TODO - Command - USER
void CommandHandler::handleUser(Client *client, const std::vector<std::string> &input) {
    std::istringstream iss(input[2] + " " + input[3]);
    std::string        user, mode, unused, realname;

    iss >> user >> mode >> unused;
    std::getline(iss >> std::ws, realname);

    if (realname.empty() || user.empty()) {
        client->sendNumericReply("461", ERR_NEEDMOREPARAMS("USER"));
        return;
    }

    if (client->getIsRegistered()) {
        client->sendNumericReply("462", ERR_ALREADYREGISTRED);
        return;
    }

    client->setUsername(user);
    client->setRealname(realname);

    if (!client->getNickname().empty() && client->getNickname() != "*" &&
        !client->getIsRegistered() && client->getIsAuthenticaded())
        welcomeMsg(client);

    std::cout << client->getNickname() << " registered with username " << client->getUsername()
              << " and realname " << client->getRealname() << std::endl;
}

static bool isValidNickname(const std::string &nick) {
    if (nick.length() < 1 || nick.length() > 9) {
        return false;
    }
    if (!std::isalpha(nick[0]) && nick[0] != '[' && nick[0] != '{' && nick[0] != '\\' &&
        nick[0] != ']' && nick[0] != '}' && nick[0] != '|' && nick[0] != '_') {
        return false;
    }
    for (size_t i = 1; i < nick.length(); ++i) {
        if (!std::isalnum(nick[i]) && nick[i] != '-' && nick[i] != '[' && nick[i] != ']' &&
            nick[i] != '{' && nick[i] != '}' && nick[i] != '\\' && nick[i] != '|' &&
            nick[i] != '_') {
            return false;
        }
    }
    return true;
}

// TODO - Command - NICK
void CommandHandler::handleNick(Client *client, const std::vector<std::string> &input) {

    if (!client->getIsAuthenticaded()) {
        client->sendNumericReply("451", ERR_NOTREGISTERED(client->getNickname()));
        return;
    }

    if (input[2].length() == 0) {
        client->sendNumericReply("431", ERR_NONICKNAMEGIVEN);
        return;
    }

    if (!isValidNickname(input[2])) {
        client->sendNumericReply("432", ERR_ERRONEUSNICKNAME(input[2]));
        return;
    }

    // Check if the nickname is already used
    std::vector<Client *> clients = _server->getClients();
    for (size_t i = 0; i < clients.size(); ++i) {
        if (clients[i] != client && clients[i]->getNickname() == input[2]) {
            client->sendNumericReply("433", ERR_NICKNAMEINUSE(input[2]));
            return;
        }
    }

    std::string oldNick = client->getNickname();
    client->setNickname(input[2]);

    if (client->getIsRegistered()) {
        std::string nickChangeMessage = ":" + oldNick + "!" + client->getUsername() + "@" +
                                        client->getHostname() + " NICK :" + input[2] + "\r\n";

        for (size_t i = 0; i < clients.size(); ++i) {
            clients[i]->sendMessage(nickChangeMessage);
        }
    }

    if (!client->getUsername().empty() && client->getNickname() != "*" &&
        !client->getIsRegistered() && client->getIsAuthenticaded())
        welcomeMsg(client);

    std::cout << "[" << client->getHostname() << "] Nickname set to " << input[2] << std::endl;
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

// TODO - Command - JOIN
std::string getMemberList(Channel *channel) {
    std::string              memberList;
    std::map<Client *, bool> members = channel->getMembers();
    for (std::map<Client *, bool>::iterator it = members.begin(); it != members.end(); ++it) {
        Client *member = it->first;
        if (channel->isOperator(member)) {
            memberList += "@";
        }
        memberList += member->getNickname() + " ";
    }
    return memberList;
}

// Commande JOIN
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
            channel->addMember(client, true);

            std::string joinMessage = ":" + client->getNickname() + "!" + client->getUsername() +
                                      "@" + client->getHostname() + " JOIN :" + channelName +
                                      "\r\n";

            std::string modeMessage = ":" + _server->getHostname() + " MODE " + channelName +
                                      " +o " + client->getNickname() + "\r\n";
            client->sendMessage(modeMessage);

            std::map<Client *, bool> members = channel->getMembers();
            for (std::map<Client *, bool>::iterator it = members.begin(); it != members.end();
                 ++it) {
                Client *member = it->first;
                member->sendMessage(joinMessage);
            }

        } else {
            if (channel->isInviteOnly()) {
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
            std::map<Client *, bool> members = channel->getMembers();
            for (std::map<Client *, bool>::iterator it = members.begin(); it != members.end();
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
}

// TODO - Command - MODE
void CommandHandler::handleMode(Client *client, const std::vector<std::string> &input) {
    std::cout << client->getNickname() << " called MODE" << std::endl;
    (void)input;
}

// TODO - Command - TOPIC
void CommandHandler::handleTopic(Client *client, const std::vector<std::string> &input) {
    std::cout << client->getNickname() << " called TOPIC" << std::endl;
    (void)input;
}

// TODO - Command - KICK
void CommandHandler::handleKick(Client *client, const std::vector<std::string> &input) {
    std::cout << client->getNickname() << " called KICK" << std::endl;
    (void)input;
}

// TODO - Command - INVITE
void CommandHandler::handleInvite(Client *client, const std::vector<std::string> &input) {
    std::cout << client->getNickname() << " called INVITE" << std::endl;
    (void)input;
}

// TODO - Command - QUIT
void CommandHandler::handleQuit(Client *client, const std::vector<std::string> &input) {
    std::cout << client->getNickname() << " called QUIT" << std::endl;
    (void)input;
}

static std::vector<std::string> commandParser(std::string input) {
    std::vector<std::string> command(4);

    std::istringstream stream(input);

    if (input[0] == ':') {
        std::getline(stream, command[0], ' ');
        command[0] = command[0].substr(1);
    }

    stream >> command[1];

    std::getline(stream >> std::ws, command[2], ':');
    if (!command[2].empty() && command[2][command[2].size() - 1] == ' ') {
        command[2].erase(command[2].size() - 1, 1);
    }
    std::getline(stream, command[3]);

    if (!command[2].empty() && command[2][command[2].length() - 1] == '\n') {
        command[2].erase(command[2].length() - 1);
    }
    return command;
}

void CommandHandler::handleCap(Client *client, const std::vector<std::string> &input) {
    if (input[2] == "LS 302") {
        client->sendNumericReply("CAP * LS :", "");
    } else if (input[2].rfind("REQ", 0) == 0) {
        client->sendNumericReply("CAP * ACK :", "");
    } else if (input[2] == "END") {
        // TODO : Handle CAP END
    } else {
        std::cerr << "Unhandled CAP command: " << input[2] << std::endl;
    }
}

CommandHandler::CommandHandler(Server *server) : _server(server) {
    // ajouter chaque fonction a la map

    _commandMap["PASS"] = &CommandHandler::handlePass;
    _commandMap["USER"] = &CommandHandler::handleUser;
    _commandMap["NICK"] = &CommandHandler::handleNick;
    _commandMap["JOIN"] = &CommandHandler::handleJoin;
    _commandMap["MODE"] = &CommandHandler::handleMode;
    _commandMap["TOPIC"] = &CommandHandler::handleTopic;
    _commandMap["KICK"] = &CommandHandler::handleKick;
    _commandMap["INVITE"] = &CommandHandler::handleInvite;
    _commandMap["PRIVMSG"] = &CommandHandler::handlePrivmsg;
    _commandMap["QUIT"] = &CommandHandler::handleQuit;
    _commandMap["CAP"] = &CommandHandler::handleCap;
    // _commandMap["WHO"] = &CommandHandler::handleWho;
}

void CommandHandler::handleCommand(Client *client, const std::string input) {

    std::vector<std::string> command = commandParser(input);
    // std::cout << "Client" << client->getFd() << " : " << input << std::endl;
    // std::cout << "hostname : " << client->getHostname() << std::endl;
    // client->sendMessage("You said: " + input + "\r\n");
    // (this->*_commandMap[command[1]])(client, command[2]);
    CommandHandlerFunctionMap::iterator it = _commandMap.find(command[1]);
    if (it != _commandMap.end())
        // if (command[1] == "USER")
        //     (this->*_commandMap[command[1]])(client, std::string(command[2] + command[3]));
        // else
        (this->*_commandMap[command[1]])(client, command);
    else {

        std::cout << client->getNickname() << " said : " << command[1];
        if (command[2] != "")
            std::cout << " " << command[2];
        std::cout << std::endl;
        // std::cout << "nope" << std::endl;
    }
}

/*
CommandHandler::handlejJoin(std::string arg)
{
}
CommandHandler::

*/
