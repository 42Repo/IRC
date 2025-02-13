#include "../../includes/Channel.h"
#include "../../includes/Server.h"
#include <algorithm>

Channel::Channel(std::string name, Client *creator, std::string password, int userLimit,
                 std::string topic)
    : _name(name),
      _topic(topic),
      _password(password),
      _userLimit(userLimit),
      _channelModes(),
      _userModes() {
    addMember(creator);
    addUserMode(creator, 'o');
}

Channel::~Channel() {
    _userModes.clear();
    _invitedUsers.clear();
}

void Channel::addMember(Client *client) {
    if (!isMember(client)) {
        _userModes[client] = std::set<char>();
        if (client->getIsOperator())
            _userModes[client].insert('o');
    }
}

void Channel::removeMember(Client *client) {
    if (isMember(client)) {
        _userModes.erase(client);

        if (_userModes.empty()) {
            client->getServer()->removeChannel(this->_name);
        }
    }
}

bool Channel::isMember(Client *client) const { return _userModes.count(client) > 0; }

void Channel::setTopic(const std::string &topic) { _topic = topic; }

void Channel::addChannelMode(char mode) { _channelModes.insert(mode); }

void Channel::removeChannelMode(char mode) { _channelModes.erase(mode); }

bool Channel::hasChannelMode(char mode) const { return _channelModes.count(mode) > 0; }

std::string Channel::getChannelModes() const {
    std::string modes;
    for (std::set<char>::const_iterator it = _channelModes.begin(); it != _channelModes.end();
         ++it) {
        modes += *it;
    }
    return modes;
}

std::string Channel::getUserModes(Client *client) const {
    std::string modes;
    if (isMember(client)) {
        for (std::set<char>::const_iterator it = _userModes.at(client).begin();
             it != _userModes.at(client).end(); ++it) {
            modes += *it;
        }
    }
    return modes;
}

void Channel::addUserMode(Client *client, char mode) {
    if (isMember(client)) {
        _userModes[client].insert(mode);
    }
}

void Channel::removeUserMode(Client *client, char mode) {
    if (isMember(client)) {
        _userModes[client].erase(mode);
        if (_userModes[client].empty()) {
            _userModes.erase(client);
        }
    }
}

bool Channel::hasUserMode(Client *client, char mode) const {
    if (isMember(client)) {
        return _userModes.at(client).count(mode) > 0;
    }
    return false;
}

// isOperator
bool Channel::isOperator(Client *client) const {
    if (isMember(client)) {
        return _userModes.at(client).count('o') > 0;
    }
    return false;
}

void Channel::addInvitedUser(std::string user) { _invitedUsers.push_back(user); }

void Channel::removeInvitedUSer(std::string user) {
    _invitedUsers.erase(std::find(_invitedUsers.begin(), _invitedUsers.end(), user));
}

std::vector<std::string> Channel::getInvitedUsers(void) { return _invitedUsers; }
