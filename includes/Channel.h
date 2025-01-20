#pragma once

#include "Client.h"
#include <map>
#include <string>

class Channel {
  public:
    Channel(std::string name);
    ~Channel();

  private:
    std::string              name;
    std::string              topic;
    std::string              password;
    std::map<Client *, bool> members;
    int                      userLimit;
    bool                     isInviteOnly;
    bool                     isTopicOperatorOnly;
};
