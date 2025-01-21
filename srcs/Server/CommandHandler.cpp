#include "../../includes/CommandHandler.h"


CommandHandler::CommandHandler()
{
  // parsing() -> split prefixe/command/arg/suffixe

  //ajouter chaque fonction a la map
  /*
    commandMap["JOIN"] = &CommandHandler::handleJoin
  */
}
void CommandHandler::handleCommand(Client *client, const std::string input)
{
  (void)client;
  std::cout << "Command handler : " << input << std::endl;
}

/*
CommandHandler::handlejJoin(std::string arg)
{
}
CommandHandler::

*/
