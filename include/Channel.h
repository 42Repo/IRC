#include "Client.h"
#include <map>
#include <string>
class Channel {
  public:
    Channel(std::string name);
    ~Channel();

  private:
    std::string              name;
    std::string              topic = NULL;
    std::string              password = NULL;
    std::map<Client *, bool> members;
    int                      userLimit = 0;
    bool                     isInviteOnly = false;
    bool                     isTopicOperatorOnly = false;
};
