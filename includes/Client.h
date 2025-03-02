#pragma once

#include <map>
#include <string>

class Server;
class Channel;

class Client {
  public:
    Client(int fd, Server *server);
    ~Client();

    void                             sendMessage(const std::string &message);
    void                             sendNumericReply(const std::string params);
    int                              getFd() const;
    std::string                      getNickname() const;
    std::string                      getUsername() const;
    std::string                      getRealname() const;
    std::string                      getHostname() const;
    std::string                      getMessageBuffer() const;
    void                             setNickname(const std::string &nickname);
    void                             appendToMessageBuffer(const std::string &data);
    void                             clearMessageBuffer();
    void                             removeFromMessageBuffer(size_t length);
    bool                             getIsAuthenticaded();
    void                             setAuthenticaded(bool value);
    bool                             getIsRegistered();
    void                             setIsRegistered(bool value);
    Server                          *getServer();
    void                             setRealname(const std::string &realname);
    void                             setUsername(const std::string &username);
    void                             joinChannel(Channel *channel);
    void                             removeChannel(std::string);
    std::map<std::string, Channel *> getChannels() const;

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
    std::map<std::string, Channel *> _channels;
    bool                             _isRegistered;
};
