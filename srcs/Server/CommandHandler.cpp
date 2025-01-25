#include "../../includes/CommandHandler.h"
#include "../../includes/Messages.h"
#include <sstream>
#include <string>
#include <vector>

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

void CommandHandler::handleNick(Client *client, const std::string &input)
{
    std::cout << "[" << client->getNickname() << "] Renamed to " << input << " !" << std::endl;  
    client->setNickname(input);
}

CommandHandler::CommandHandler(Server *server) : _server(server) {
    // ajouter chaque fonction a la map

      _commandMap["NICK"] = &CommandHandler::handleNick;
}


void CommandHandler::handleCommand(Client *client, const std::string input) {
    
    
    std::vector<std::string> command = commandParser(input);
    // std::cout << "Client" << client->getFd() << " : " << input << std::endl;
    // std::cout << "hostname : " << client->getHostname() << std::endl;
    // client->sendMessage("You said: " + input + "\r\n");
    // client->sendNumericReply(
    //     (char *)"001",
    //     std::vector<std::string>(
    //         1, RPL_WELCOME(client->getNickname(), client->getUsername(), client->getHostname())));
    // (this->*_commandMap[command[1]])(client, command[2]);
    CommandHandlerFunctionMap::iterator it = _commandMap.find(command[1]);
    if(it != _commandMap.end())
        (this->*_commandMap[command[1]])(client, command[2]);
    else
        std::cout << "nope" << std::endl;
         
}

/*
CommandHandler::handlejJoin(std::string arg)
{
}
CommandHandler::

*/
