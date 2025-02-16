#include <cstdlib>
#include <iostream>
#include <sys/poll.h>
#include <vector>

class Bot {
  public:
    Bot(std::string name, int port, std::string servAdress, std::string password);
    ~Bot();
    void        joinServer();
    void        loginToServ();
    void        sendMessage(const std::string &msg);
    void        setName(std::string name);
    std::string getName();
    void        answer(std::vector<std::string> message);

  private:
    int           _fd;
    std::string   _name;
    int           _port;
    std::string   _servAddress;
    std::string   _messageBuffer;
    std::string   _password;
    struct pollfd _pollfd;
};