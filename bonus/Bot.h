#include <arpa/inet.h>
#include <cstdlib>
#include <cstring>
#include <curl/curl.h>
#include <iostream>
#include <netinet/in.h>
#include <sstream>
#include <string>
#include <sys/poll.h>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>

static bool g_shutdown = 0;
// Message sent to Gemini
#define GEMINI_MSG(name, message)                                                                  \
    (std::string("{ \"contents\": [") +                                                            \
     "{ \"role\": \"user\", \"parts\": [ { \"text\": \"Tu es un BOT IRC qui s'appelle  " + name +  \
     "Reponds sans AUCUN markdown. Réponds dans la même langue que la question.\" } ] },"                     \
     "{ \"role\": \"user\", \"parts\": [ { \"text\": \"" +                                         \
     message + "\" } ] } ] }")

// Gemini API URL
#define GEMINI_URL(api)                                                                            \
    ("https://generativelanguage.googleapis.com/v1/models/"                                        \
     "gemini-2.0-flash:generateContent?key=" +                                                           \
     api)

class Bot {
  public:
    Bot(std::string name, int port, std::string servAdress, std::string password,
        std::string api_key);
    ~Bot();
    void        joinServer();
    void        loginToServ();
    void        sendMessage(const std::string &msg);
    void        answer(std::vector<std::string> message);
    std::string getLLMReply(std::string message);
    bool        isAlreadyJoined(std::string channel);

  private:
    int                      _fd;
    std::string              _name;
    int                      _port;
    std::string              _servAddress;
    std::string              _messageBuffer;
    std::string              _password;
    struct pollfd            _pollfd;
    std::vector<std::string> _channels;
    std::string              _api_key;
};