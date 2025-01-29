#pragma once

#include "Client.h"
#include <iostream>
#include <map>
class CommandHandler {

  public:
    typedef void (CommandHandler::*CommandHandlerFunction)(Client            *client,
                                                           const std::string &input);
    typedef std::map<std::string, CommandHandlerFunction> CommandHandlerFunctionMap;
    CommandHandler(Server *server);
    void                      handleCommand(Client *client, const std::string input);
    CommandHandlerFunctionMap _commandMap;

  protected:
  private:
    Server *_server;

    void handlePass(Client *client, const std::string &input);
    void handleUser(Client *client, const std::string &input);
    void handleNick(Client *client, const std::string &input);
    void handleJoin(Client *client, const std::string &input);
    void handleMode(Client *client, const std::string &input);
    void handleTopic(Client *client, const std::string &input);
    void handleKick(Client *client, const std::string &input);
    void handleInvite(Client *client, const std::string &input);
    void handlePrivmsg(Client *client, const std::string &input);
    void handleQuit(Client *client, const std::string &input);
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
