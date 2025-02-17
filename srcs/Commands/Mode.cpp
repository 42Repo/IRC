#include "../../includes/CommandHandler.h"
#include "../../includes/Error.h"
#include "../../includes/Server.h"

void CommandHandler::handleMode(Client *client, const std::vector<std::string> &input) {
    try {
        if (input.size() < 3 || input[2].empty()) {
            throw Error::IRCError(ERR_NEEDMOREPARAMS("MODE").c_str());
        }
        if (!client->getIsRegistered()) {
            throw Error::IRCError(ERR_NOTREGISTERED(client->getNickname()).c_str());
        }

        std::string target = input[2];
        std::string modeString = "";
        std::string argument = "";
        std::string first_arg = "";

        // Separer la cible et les modes/arguments
        size_t spacePos = target.find(' ');
        if (spacePos != std::string::npos) {
            modeString = target.substr(spacePos + 1);
            target = target.substr(0, spacePos);
        }
        // Si il y a un argument
        size_t spacePos2 = modeString.find(' ');
        if (spacePos2 != std::string::npos) {
            argument = modeString.substr(spacePos2 + 1);
            modeString = modeString.substr(0, spacePos2);
        }

        if (target[0] == '#') { // Cible = channel
            Channel *channel = _server->getChannelByName(target);
            if (!channel) {
                throw Error::IRCError(ERR_NOSUCHCHANNEL(client->getNickname(), target).c_str());
            }

            if (!channel->isMember(client)) {
                throw Error::IRCError(ERR_NOTONCHANNEL(client->getNickname(), target).c_str());
            }

            if (modeString.empty()) {
                std::map<Client *, std::set<char> > members = channel->getMembers();
                std::string                        userModesString;
                std::string                        serverModesString;
                for (std::map<Client *, std::set<char> >::const_iterator it = members.begin();
                     it != members.end(); ++it) {
                    userModesString = channel->getUserModes(it->first);
                    if (userModesString.find('o') != std::string::npos) {
                        userModesString.erase(userModesString.find('o'), 1);
                    }
                    serverModesString = channel->getChannelModes();
                    if (!userModesString.empty() || !serverModesString.empty()) {
                        client->sendNumericReply("324 " + client->getNickname() + " " +
                                                 channel->getName() + " " + "+" +
                                                 serverModesString + userModesString);
                    }
                }
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
                    if (channel->isOperator(client)) {
                        if (adding) {
                            if (channel->hasChannelMode('i')) {
                                continue;
                            }
                            channel->addChannelMode('i');
                        } else {
                            if (!channel->hasChannelMode('i')) {
                                continue;
                            }
                            channel->removeChannelMode('i');
                        }
                        std::string modeMessage =
                            ":" + client->getNickname() + "!" + client->getUsername() + "@" +
                            client->getHostname() + " MODE " + channel->getName() + " " +
                            (adding ? "+i" : "-i") + "\r\n";
                        std::map<Client *, std::set<char> > members = channel->getMembers();
                        for (std::map<Client *, std::set<char> >::iterator it = members.begin();
                             it != members.end(); ++it) {
                            Client *member = it->first;
                            member->sendMessage(modeMessage);
                        }
                    } else {
                        throw Error::IRCError(
                            ERR_CHANOPRIVSNEEDED(client->getNickname(), channel->getName())
                                .c_str());
                    }
                    break;

                case 't':
                    if (channel->isOperator(client)) {
                        if (adding) {
                            if (channel->hasChannelMode('t')) {
                                continue;
                            }
                            channel->addChannelMode('t');
                        } else {
                            if (!channel->hasChannelMode('t')) {
                                continue;
                            }
                            channel->removeChannelMode('t');
                        }
                        std::string modeMessage =
                            ":" + client->getNickname() + "!" + client->getUsername() + "@" +
                            client->getHostname() + " MODE " + channel->getName() + " " +
                            (adding ? "+t" : "-t") + "\r\n";
                        std::map<Client *, std::set<char> > members = channel->getMembers();
                        for (std::map<Client *, std::set<char> >::iterator it = members.begin();
                             it != members.end(); ++it) {
                            Client *member = it->first;
                            member->sendMessage(modeMessage);
                        }
                    } else {
                        throw Error::IRCError(
                            ERR_CHANOPRIVSNEEDED(client->getNickname(), channel->getName())
                                .c_str());
                    }
                    break;

                case 'k':
                    first_arg = argument.substr(0, argument.find(' '));
                    argument = argument.substr(argument.find(' ') + 1);
                    if (channel->isOperator(client)) {
                        if (adding) {
                            channel->addChannelMode('k');
                            if (argument.empty()) // Si il n'y a pas d'argument alors que +k alors
                                                  // erreur
                                throw Error::IRCError(ERR_NEEDMOREPARAMS("MODE").c_str());
                            channel->setPassword(argument);
                        } else {
                            channel->removeChannelMode('k');
                            channel->setPassword("");
                        }
                        std::string modeMessage =
                            ":" + client->getNickname() + "!" + client->getUsername() + "@" +
                            client->getHostname() + " MODE " + channel->getName() + " " +
                            (adding ? "+k" : "-k") + " " + first_arg + "\r\n";
                        std::map<Client *, std::set<char> > members = channel->getMembers();
                        for (std::map<Client *, std::set<char> >::iterator it = members.begin();
                             it != members.end(); ++it) {
                            Client *member = it->first;
                            member->sendMessage(modeMessage);
                        }
                    } else {
                        throw Error::IRCError(
                            ERR_CHANOPRIVSNEEDED(client->getNickname(), channel->getName())
                                .c_str());
                    }
                    break;

                case 'o':
                    first_arg = argument.substr(0, argument.find(' '));
                    argument = argument.substr(argument.find(' ') + 1);
                    if (channel->isOperator(client)) {
                        // MODE #channel +o nickname

                        if (first_arg.empty()) {
                            throw Error::IRCError(ERR_NEEDMOREPARAMS("MODE").c_str());
                        }

                        Client               *targetClient = NULL;
                        std::vector<Client *> clients = _server->getClients();
                        for (size_t j = 0; j < clients.size(); ++j) {
                            if (clients[j]->getNickname() == first_arg) {
                                targetClient = clients[j];
                                break;
                            }
                        }

                        if (targetClient) {
                            if (channel->isMember(
                                    targetClient)) { // On verifie si le client est dans le channel
                                if (adding) {
                                    channel->addUserMode(targetClient, 'o');

                                } else {
                                    channel->removeUserMode(targetClient, 'o');
                                }
                                std::string modeMessage =
                                    ":" + client->getNickname() + "!" + client->getUsername() +
                                    "@" + client->getHostname() + " MODE " + channel->getName() +
                                    " " + (adding ? "+o" : "-o") + " " + first_arg + "\r\n";
                                std::map<Client *, std::set<char> > members = channel->getMembers();
                                for (std::map<Client *, std::set<char> >::iterator it =
                                         members.begin();
                                     it != members.end(); ++it) {
                                    Client *member = it->first;
                                    member->sendMessage(modeMessage);
                                }
                            } else
                                throw Error::IRCError(
                                    ERR_NOTONCHANNEL(client->getNickname(), first_arg)
                                        .c_str()); // Si le client n'est pas dans le channel
                        } else
                            throw Error::IRCError(ERR_NOSUCHNICK(client->getNickname(), first_arg)
                                                      .c_str()); // Si il n'existe pas

                    } else {
                        throw Error::IRCError(
                            ERR_CHANOPRIVSNEEDED(client->getNickname(), channel->getName())
                                .c_str());
                    }
                    break;

                case 'l':
                    first_arg = argument.substr(0, argument.find(' '));
                    argument = argument.substr(argument.find(' ') + 1);
                    if (channel->isOperator(client)) {
                        if (adding) {
                            channel->addChannelMode('l');
                            if (first_arg.empty()) // Si il n'y a pas d'argument alors que +k alors
                                throw Error::IRCError(ERR_NEEDMOREPARAMS("MODE").c_str());
                            int limit = std::atoi(first_arg.c_str());
                            if (limit > 0)
                                channel->setUserLimit(limit);
                            else
                                throw Error::IRCError(
                                    ERR_NEEDMOREPARAMS("MODE").c_str()); // erreur de syntaxe
                        } else {
                            channel->removeChannelMode('l');
                            channel->setUserLimit(0); // On enleve le password
                        }
                        // Notification
                        std::string modeMessage =
                            ":" + client->getNickname() + "!" + client->getUsername() + "@" +
                            client->getHostname() + " MODE " + channel->getName() + " " +
                            (adding ? "+l" : "-l") + " " + first_arg + "\r\n";
                        std::map<Client *, std::set<char> > members = channel->getMembers();
                        for (std::map<Client *, std::set<char> >::iterator it = members.begin();
                             it != members.end(); ++it) {
                            Client *member = it->first;
                            member->sendMessage(modeMessage);
                        }

                    } else {
                        throw Error::IRCError(
                            ERR_CHANOPRIVSNEEDED(client->getNickname(), channel->getName())
                                .c_str());
                    }
                    break;

                default:
                    client->sendNumericReply(ERR_UNKNOWNMODE(modeChar, client->getNickname()));
                    break;
                }
            }

        } else { // Cible = utilisateur (modes utilisateur)
                 // Pour l'instant on ne gere pas
            client->sendNumericReply(ERR_UNKNOWNMODE(modeString, client->getNickname()));
            return;
        }
    } catch (const std::exception &e) {
        Error(e.what(), client);
    }
}
