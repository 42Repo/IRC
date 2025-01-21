#include "../../includes/CommandHandler.h"
#include <vector>

static std::vector<std::string> commandParser(std::string input) {
    std::vector<std::string> command(4);
    (void)input;
    return command;
}

CommandHandler::CommandHandler(Server *server) : _server(server) {
    // parsing() -> split prefixe/command/arg/suffixe

    // ajouter chaque fonction a la map
    /*
      commandMap["JOIN"] = &CommandHandler::handleJoin
    */
}
void CommandHandler::handleCommand(Client *client, const std::string input) {
    std::cout << "Client" << client->getFd() << " : " << input << std::endl;
    std::cout << "hostname : " << client->getHostname() << std::endl;
    std::vector<std::string> command = commandParser(input);
}

/*
CommandHandler::handlejJoin(std::string arg)
{
}
CommandHandler::

*/
