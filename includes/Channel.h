#pragma once

#include "Client.h"
#include <map>
#include <set>

class Channel {
  public:
    Channel(std::string name, Client *creator, std::string password = "", int userLimit = 0,
            std::string topic = "");
    ~Channel();

    void addMember(Client *client);
    void removeMember(Client *client);
    bool isMember(Client *client) const;

    std::string getName() const { return _name; }
    std::string getTopic() const { return _topic; }
    void        setTopic(const std::string &topic);
    std::string getPassword() const { return _password; }
    int         getUserLimit() const { return _userLimit; }
    bool        isOperator(Client *client) const;
    void        setUserLimit(int limit) { _userLimit = limit; }
    void        setPassword(const std::string &password) { _password = password; }
    void        addChannelMode(char mode);
    void        removeChannelMode(char mode);
    bool        hasChannelMode(char mode) const;
    std::string getChannelModes() const;
    std::string getUserModes(Client *client) const;
    void        addUserMode(Client *client, char mode);
    void        removeUserMode(Client *client, char mode);
    bool        hasUserMode(Client *client, char mode) const;
    std::map<Client *, std::set<char> > getMembers() const { return _userModes; }

  private:
    std::string                        _name;
    std::string                        _topic;
    std::string                        _password;
    int                                _userLimit;
    std::set<char>                     _channelModes;
    std::map<Client *, std::set<char> > _userModes;
};
