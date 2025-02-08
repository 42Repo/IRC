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
    void        sendNumericReply(const char *numericStr, const std::string params);
    int         getFd() const;
    std::string getNickname() const;
    std::string getUsername() const;
    std::string getRealname() const;
    std::string getHostname() const;
    std::string getMessageBuffer() const;
    std::map<std::string, Channel *> getChannels() const;
    void        setNickname(const std::string &nickname) { _nickname = nickname; }
    void        appendToMessageBuffer(const std::string &data);
    void        clearMessageBuffer();
    void        removeFromMessageBuffer(size_t length);
    bool        getIsAuthenticaded() { return _isAuthenticated; };
    void        setAuthenticaded(bool value) { _isAuthenticated = value; };
    bool        getIsRegistered() { return _isRegistered; };
    void        setIsRegistered(bool value) { _isRegistered = value; };
    void        setIsOperator(bool value) { _isOperator = value; };
    bool        getIsOperator() { return _isOperator; };
    Server      *getServer();
    void        setRealname(const std::string &realname) { _realname = realname; };
    void        setUsername(const std::string &username) { _username = username; };
    void        setHostname(const std::string &hostname) { _hostname = hostname; };
    void        joinChannel(Channel *channel);
    void        leaveChannel(Channel *channel);
    void        addMode(char mode);
    void        removeMode(char mode);
    std::string getUserModes() const { return _userModes; };

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
    bool                             _isRegistered;
    std::string                      _userModes;
};
