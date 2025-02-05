#include "../../includes/Channel.h"

Channel::Channel(std::string name, Client *creator, std::string password, int userLimit,
                 bool isInviteOnly, bool isTopicOperatorOnly, std::string modes)
    : _name(name),
      _topic(""),
      _password(password),
      _members(),
      _userLimit(userLimit),
      _isInviteOnly(isInviteOnly),
      _isTopicOperatorOnly(isTopicOperatorOnly),
      _modes(modes) {
    addMember(creator, true);
}

Channel::~Channel() {}

void Channel::removeMember(Client *client) {
    if (_members.find(client) != _members.end()) {
        _members.erase(client);
    }
}

void Channel::addMember(Client *client, bool isOperator) { _members[client] = isOperator; }

bool Channel::isOperator(Client *client) const {
    std::map<Client *, bool>::const_iterator it = _members.find(client);
    if (it != _members.end()) {
        return it->second;
    }
    return false;
}

bool Channel::isMember(Client *client) const { return _members.find(client) != _members.end(); }
