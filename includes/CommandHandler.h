#pragma once

#include <iostream>
#include <map>
#include "Client.h"
class CommandHandler {

  public:
    CommandHandler(Server *server);
	static void handleCommand(Client *client, const std::string input);

  private:
    Server *_server;
    std::map<std::string, void (CommandHandler::*)(Client *client, const std::string &)> commandMap;

    // ajouter chaque commande
    /*
            PASS
            NICK
            USER
            JOIN
            PRIVMSG
            QUIT
    */
};
