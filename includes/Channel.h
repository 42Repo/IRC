#pragma once

#include "Client.h"
#include <map>

class Channel {
  public:
    Channel(std::string name, Client *creator, std::string password = "", int userLimit = 0,
            bool isInviteOnly = false, bool isTopicOperatorOnly = false, std::string modes = "");
    ~Channel();

    void                     addMember(Client *client, bool isOperator = false);
    void                     removeMember(Client *client);
    std::string              getName() const { return _name; }
    std::string              getTopic() const { return _topic; }
    void                     setTopic(const std::string &topic) { _topic = topic; }
    std::string              getPassword() const { return _password; }
    bool                     isInviteOnly() const { return _isInviteOnly; }
    bool                     isTopicOperatorOnly() const { return _isTopicOperatorOnly; }
    int                      getUserLimit() const { return _userLimit; }
    std::string              getModes() const { return _modes; }
    bool                     isOperator(Client *client) const;
    bool                     isMember(Client *client) const;
    std::map<Client *, bool> getMembers() const { return _members; }

  private:
    std::string              _name;
    std::string              _topic;
    std::string              _password;
    std::map<Client *, bool> _members; // true if operator
    int                      _userLimit;
    bool                     _isInviteOnly;
    bool                     _isTopicOperatorOnly;
    std::string              _modes;
};
