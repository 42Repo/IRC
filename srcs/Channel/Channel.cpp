#include "../../includes/Channel.h"

Channel::Channel(std::string name, std::string password, int userLimit, bool isInviteOnly,
                 bool isTopicOperatorOnly, std::string modes)
    : _name(name),
      _password(password),
      _userLimit(userLimit),
      _isInviteOnly(isInviteOnly),
      _isTopicOperatorOnly(isTopicOperatorOnly),
      _modes(modes) {}

void Channel::removeMember(Client *client) { _members.erase(client); }

void Channel::addMember(Client *client, bool isOperator) { _members[client] = isOperator; }
