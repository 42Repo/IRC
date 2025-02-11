#pragma once

#include "Client.h"
#include "Error.hpp"
#include "Messages.h"
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

class Server;
class CommandHandler {

  public:
    typedef void (CommandHandler::*CommandHandlerFunction)(Client                         *client,
                                                           const std::vector<std::string> &input);
    typedef std::map<std::string, CommandHandlerFunction> CommandHandlerFunctionMap;
    CommandHandler(Server *server);
    void                      handleCommand(Client *client, const std::string input);
    CommandHandlerFunctionMap _commandMap;

  protected:
  private:
    Server *_server;

    void handlePass(Client *client, const std::vector<std::string> &input);
    void handleUser(Client *client, const std::vector<std::string> &input);
    void handleNick(Client *client, const std::vector<std::string> &input);
    void handleJoin(Client *client, const std::vector<std::string> &input);
    void handleMode(Client *client, const std::vector<std::string> &input);
    void handleTopic(Client *client, const std::vector<std::string> &input);
    void handleKick(Client *client, const std::vector<std::string> &input);
    void handleInvite(Client *client, const std::vector<std::string> &input);
    void handlePrivmsg(Client *client, const std::vector<std::string> &input);
    void handleQuit(Client *client, const std::vector<std::string> &input);
    void handleCap(Client *client, const std::vector<std::string> &input);
    void handleWho(Client *client, const std::vector<std::string> &input);
    void handleUserMode(Client *client, const std::vector<std::string> &input);
    void handlePing(Client *client, const std::vector<std::string> &input);
    void welcomeMsg(Client *client);

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