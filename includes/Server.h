#pragma once

#include "Channel.h"
#include "CommandHandler.h"
#include "Messages.h"
#include <poll.h>
#include <vector>

class Server {
  public:
    Server(int port, const std::string &password);
    ~Server();

    void                  run();
    void                  setupServerSocket();
    std::string           getHostname();
    std::string           getPassword() { return _password; };
    void                  removeClient(Client *client);
    std::vector<Client *> getClients() { return _clients; };

  private:
    Server();
    Server(const Server &server);
    Server &operator=(const Server &server);

    int                              _port;
    std::string                      _password;
    int                              _server_fd;
    std::vector<Client *>            _clients;
    std::map<std::string, Channel *> _channels;
    std::vector<pollfd>              _fds;
    time_t                           _creationTime;
    CommandHandler                   _commandHandler;
};
