#pragma once

#include "Channel.h"
#include <poll.h>
#include <vector>

class Server {
  public:
    Server(int port, std::string password);
    ~Server();

    void     run();
    void     setupServerSocket();

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
};
