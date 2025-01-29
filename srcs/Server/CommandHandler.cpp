#include "../../includes/CommandHandler.h"
#include "../../includes/Messages.h"
#include <sstream>
#include <string>
#include <vector>

// TODO - Command - PASS
void CommandHandler::handlePass(Client *client, const std::string &input) {
    std::cout << client->getNickname() << " called PASS" << std::endl;
    (void)input;
}

// TODO - Command - USER
void CommandHandler::handleUser(Client *client, const std::string &input) {
    std::cout << client->getNickname() << " called USER" << std::endl;
    (void)input;
}

// TODO - Command - NICK
void CommandHandler::handleNick(Client *client, const std::string &input) {
    std::cout << "[" << client->getNickname() << "] Renamed to " << input << " !" << std::endl;
    client->setNickname(input);
}

// TODO - Command - JOIN
void CommandHandler::handleJoin(Client *client, const std::string &input) {
    std::cout << client->getNickname() << " called JOIN" << std::endl;
    (void)input;
}

// TODO - Command - MODE
void CommandHandler::handleMode(Client *client, const std::string &input) {
    std::cout << client->getNickname() << " called MODE" << std::endl;
    (void)input;
}

// TODO - Command - TOPIC
void CommandHandler::handleTopic(Client *client, const std::string &input) {
    std::cout << client->getNickname() << " called TOPIC" << std::endl;
    (void)input;
}

// TODO - Command - KICK
void CommandHandler::handleKick(Client *client, const std::string &input) {
    std::cout << client->getNickname() << " called KICK" << std::endl;
    (void)input;
}

// TODO - Command - INVITE
void CommandHandler::handleInvite(Client *client, const std::string &input) {
    std::cout << client->getNickname() << " called INVITE" << std::endl;
    (void)input;
}

// TODO - Command - PRIVMSG
void CommandHandler::handlePrivmsg(Client *client, const std::string &input) {
    std::cout << client->getNickname() << " called PRIVATEMSG" << std::endl;
    (void)input;
}

// TODO - Command - QUIT
void CommandHandler::handleQuit(Client *client, const std::string &input) {
    std::cout << client->getNickname() << " called QUI" << std::endl;
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
    std::getline(stream, command[3]);

    if (!command[2].empty() && command[2][command[2].length() - 1] == '\n') {
        command[2].erase(command[2].length() - 1);
    }
    return command;
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
}

void CommandHandler::handleCommand(Client *client, const std::string input) {

    std::vector<std::string> command = commandParser(input);
    // std::cout << "Client" << client->getFd() << " : " << input << std::endl;
    // std::cout << "hostname : " << client->getHostname() << std::endl;
    // client->sendMessage("You said: " + input + "\r\n");
    // (this->*_commandMap[command[1]])(client, command[2]);
    CommandHandlerFunctionMap::iterator it = _commandMap.find(command[1]);
    if (it != _commandMap.end())
        (this->*_commandMap[command[1]])(client, command[2]);
    else {

        std::cout << client->getNickname() << "said : " << command[1];
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
