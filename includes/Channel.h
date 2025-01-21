#pragma once

#include "Client.h"
#include <map>

class Channel {
  public:
    Channel(std::string name);
    ~Channel();

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
