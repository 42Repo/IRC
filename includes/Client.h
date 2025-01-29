#pragma once

#include <map>
#include <string>
#include <vector>

class Server;
class Channel;

class Client {
  public:
    Client(int fd, Server *server);
    ~Client();
    void        sendMessage(const std::string &message);
    void        sendNumericReply(const char *numericStr, const std::vector<std::string> params);
    int         getFd();
    std::string getNickname();
    std::string getUsername();
    std::string getRealname();
    std::string getHostname();
    void        setNickname(std::string nickname) { _nickname = nickname; }

  private:
    Client();
    Client(const Client &client);
    Client &operator=(const Client &client);

    int                              _fd;
    Server                          *_server;
    std::string                      _nickname;
    std::string                      _username;
    std::string                      _realname;
    std::string                      _hostname;
    bool                             _isAuthenticated;
    std::string                      _messageBuffer;
    bool                             _isOperator;
    std::map<std::string, Channel *> _channels;
    std::string                      _status;
};
