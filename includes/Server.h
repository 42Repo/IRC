#pragma once

#include "Channel.h"
#include "CommandHandler.h"
#include "Messages.h"
#include <csignal>
#include <poll.h>
#include <vector>

class Server {
  public:
    Server(int port, const std::string &password);
    ~Server();

    void                             run();
    void                             setupServerSocket();
    std::string                      getHostname();
    std::string                      getPassword();
    void                             removeClient(Client *client);
    std::vector<Client *>            getClients();
    std::map<std::string, Channel *> getChannels();
    Channel                         *getChannelByName(const std::string &name);
    void                             addChannel(Channel *channel);
    void                             removeChannel(const std::string &name);
    Client                          *getClientByName(std::string client);
    void                             setShutdownFlag(bool value);

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
    volatile sig_atomic_t            _shutdown;
};
