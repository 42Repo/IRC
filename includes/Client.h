#include <string>

class Client {
  public:
    Client(int fd);
    ~Client();

  private:
    Client();
    Client(const Client &client);
    Client &operator=(const Client &client);

    int         fd;
    std::string nickname;
    std::string username;
    std::string realname;
    std::string hostname;
    bool        isAuthenticated;
    std::string messageBuffer;
    bool        isOperator;
};
