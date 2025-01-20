#include "../../includes/Server.h"

Server::Server(int port, std::string password)
    : _port(port),
      _password(password),
      _server_fd(0),
      _clients(0),
      _fds(0) {}

Server::~Server() {}

void Server::setupServerSocket() {};
void Server::run() {};