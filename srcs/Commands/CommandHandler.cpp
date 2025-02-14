#include "../../includes/CommandHandler.h"
#include "../../includes/Server.h"
#include <exception>

void CommandHandler::welcomeMsg(Client *client) {
    client->setIsRegistered(true);

    client->sendNumericReply(
        RPL_WELCOME(client->getNickname(), client->getUsername(), client->getHostname()));
    client->sendNumericReply(RPL_YOURHOST(client->getNickname(), _server->getHostname(), "1.0"));
    client->sendNumericReply(RPL_CREATED(client->getNickname(), "DATE"));
    client->sendNumericReply(
        RPL_MYINFO(client->getNickname(), _server->getHostname(), "1.0", "", ""));

    client->sendNumericReply(RPL_MOTDSTART(client->getNickname()));
    client->sendNumericReply(RPL_MOTD(client->getNickname()));
    client->sendNumericReply(RPL_MOTDEND(client->getNickname()));
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
        client->sendNumericReply("CAP * LS :");
    } else if (input[2].rfind("REQ", 0) == 0) {
        client->sendNumericReply("CAP * ACK :");
    } else if (input[2] == "END") {
        // TODO : Handle CAP END
    } else {
        std::cerr << "Unhandled CAP command: " << input[2] << std::endl;
    }
}

void CommandHandler::handlePing(Client *client, const std::vector<std::string> &input) {
    if (input.size() < 3 || input[2].empty()) {
        client->sendNumericReply(" 409 :No origin specified");
        return;
    }

    std::string pongResponse =
        ":" + _server->getHostname() + " PONG " + _server->getHostname() + " :" + input[2] + "\r\n";
    client->sendMessage(pongResponse);
}

CommandHandler::CommandHandler(Server *server) : _server(server) {
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
    _commandMap["PING"] = &CommandHandler::handlePing;
    _commandMap["WHO"] = &CommandHandler::handleWho;
    _commandMap["LIST"] = &CommandHandler::handleList;
}

void CommandHandler::handleCommand(Client *client, const std::string input) {

    std::vector<std::string> command = commandParser(input);
    // std::cout << "Client" << client->getFd() << " : " << input << std::endl;
    // std::cout << "hostname : " << client->getHostname() << std::endl;
    // client->sendMessage("You said: " + input + "\r\n");
    // (this->*_commandMap[command[1]])(client, command[2]);
    CommandHandlerFunctionMap::iterator it = _commandMap.find(command[1]);
    if (it != _commandMap.end())
        try {
            (this->*_commandMap[command[1]])(client, command);
        } catch (const std::exception &e) {
            Error(e.what(), client);
        }
    else {

        std::cout << client->getNickname() << " said : " << command[1];
        if (command[2] != "")
            std::cout << " " << command[2];
        std::cout << std::endl;
        // std::cout << "nope" << std::endl;
    }
}
