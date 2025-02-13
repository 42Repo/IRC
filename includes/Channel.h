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
    void                               addInvitedUser(std::string user);
    void                               removeInvitedUSer(std::string user);
    std::vector<std::string>           getInvitedUsers(void);
    time_t                             getTimestamp();
    void                               setTimestamp(time_t timestamp);
      void                              setTopicSetter(std::string setter) { _topicSetter = setter; };
      std::string                       getTopicSetter(void){ return _topicSetter; };
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
