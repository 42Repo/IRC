#include "Channel.h"
#include <iostream>
#include <map>
#include <poll.h>
#include <string>
#include <vector>

class Server {
  public:
    Server(int port, std::string password);
    ~Server();

    void start();
    void stop();

  private:
    Server();
    Server(const Server &server);
    Server &operator=(const Server &server);

    int                              port;
    std::string                      password;
    int                              server_fd;
    std::vector<Client *>            clients;
    std::map<std::string, Channel *> channels;
    std::vector<pollfd>              fds;
};
