#pragma once

#include "Client.h"
#include <map>

class Channel {
  public:
    Channel(std::string name, std::string password = "", int userLimit = 0,
            bool isInviteOnly = false, bool isTopicOperatorOnly = false, std::string modes = "");
    ~Channel();

    void addMember(Client *client, bool isOperator = false);
    void removeMember(Client *client);

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
