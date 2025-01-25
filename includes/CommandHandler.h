#pragma once

#include "Client.h"
#include <iostream>
#include <map>
class CommandHandler {

  public:
  typedef  void (CommandHandler::*CommandHandlerFunction)(Client *client, const std::string &input) ;
  typedef std::map<std::string, CommandHandlerFunction>  CommandHandlerFunctionMap;
    CommandHandler(Server *server);
    void handleCommand(Client *client, const std::string input);
    CommandHandlerFunctionMap _commandMap;

protected:
  private:
    Server                                                                              *_server;

    void handleNick(Client *client, const std::string &input);
    // ajouter chaque commande
    /*
            PASS
            USER
            NICK
            JOIN
            MODE
            TOPIC
            KICK
            INVIsTE
            PRIVMSG
            QUIT
    */
};
