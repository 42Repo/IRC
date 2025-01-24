#pragma once

#include "Client.h"
#include <iostream>
#include <map>
class CommandHandler {

  public:
    CommandHandler(Server *server);
    void handleCommand(Client *client, const std::string input);
    std::map<std::string, void (CommandHandler::*)(Client *client, const std::string &input)> _commandMap;

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
