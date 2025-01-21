#pragma once

#include <iostream>
#include <map>
#include "Client.h"
class CommandHandler {

  public:
    CommandHandler(Client *client, std::string &input);

  private:
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