#include <string>

class Server {
  public:
    Server(int port, std::string password);
    ~Server();

    void start();
    void stop();

  private:
    Server();
    Server(const Server &server);
    Server &operator=(const Server &server);

    int         port;
    std::string password;
};
