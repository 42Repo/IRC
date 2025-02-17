#pragma once

#include "Client.h"
#include <ctime>
#include <map>
#include <set>
#include <string>
#include <vector>

class Channel {
  public:
    Channel(std::string name, Client *creator, std::string password = "", int userLimit = 0,
            std::string topic = "");
    ~Channel();

    void addMember(Client *client);
    void removeMember(Client *client);
    bool isMember(Client *client) const;

    std::string                         getName() const;
    std::string                         getTopic() const;
    void                                setTopic(const std::string &topic);
    std::string                         getPassword() const;
    void                                setPassword(const std::string &password);
    int                                 getUserLimit() const;
    void                                setUserLimit(int limit);
    bool                                isOperator(Client *client) const;
    void                                addChannelMode(char mode);
    void                                removeChannelMode(char mode);
    bool                                hasChannelMode(char mode) const;
    std::string                         getChannelModes() const;
    std::string                         getUserModes(Client *client) const;
    void                                addUserMode(Client *client, char mode);
    void                                removeUserMode(Client *client, char mode);
    bool                                hasUserMode(Client *client, char mode) const;
    void                                setTopicSetter(std::string setter);
    std::string                         getTopicSetter();
    void                                addInvitedUser(std::string user);
    std::vector<std::string>            getInvitedUsers();
    time_t                              getTimestamp();
    void                                setTimestamp(time_t timestamp);
    std::map<Client *, std::set<char> > getMembers() const;

  private:
    std::string                         _name;
    std::string                         _topic;
    std::string                         _password;
    int                                 _userLimit;
    std::set<char>                      _channelModes;
    std::map<Client *, std::set<char> > _userModes;
    std::vector<std::string>            _invitedUsers;
    time_t                              _timestamp;
    std::string                         _topicSetter;
};
