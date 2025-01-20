#pragma once

#include <string>

class Client {
  public:
    Client(int fd);
    ~Client();

  private:
    Client();
    Client(const Client &client);
    Client &operator=(const Client &client);

    int         _fd;
    std::string _nickname;
    std::string _username;
    std::string _realname;
    std::string _hostname;
    bool        _isAuthenticated;
    std::string _messageBuffer;
    bool        _isOperator;
};
