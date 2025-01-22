#include "../../includes/CommandHandler.h"
#include "../../includes/Messages.h"
#include <sstream>
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

    return command;
}

CommandHandler::CommandHandler(Server *server) : _server(server) {
    // ajouter chaque fonction a la map
    /*
      commandMap["JOIN"] = &CommandHandler::handleJoin
    */
}

void CommandHandler::handleCommand(Client *client, const std::string input) {
    std::cout << "Client" << client->getFd() << " : " << input << std::endl;
    std::cout << "hostname : " << client->getHostname() << std::endl;
    client->sendMessage("You said: " + input + "\r\n");
    client->sendNumericReply(
        (char *)"001",
        std::vector<std::string>(
            1, RPL_WELCOME(client->getNickname(), client->getUsername(), client->getHostname())));
    std::vector<std::string> command = commandParser(input);
}

/*
CommandHandler::handlejJoin(std::string arg)
{
}
CommandHandler::

*/
