#pragma once

#include "Client.h"
#include <iostream>
#include <map>
class CommandHandler {

  public:
    CommandHandler(Server *server);
    static void handleCommand(Client *client, const std::string input);

  private:
    Server                                                                              *_server;
    std::map<std::string, void (CommandHandler::*)(Client *client, const std::string &)> commandMap;

    // ajouter chaque commande
    /*
            PASS
            USER
            NICK
            JOIN
            MODE
            TOPIC
            KICK
            INVITE
            PRIVMSG
            QUIT
    */
};
