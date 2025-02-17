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

// Message sent to Gemini
#define GEMINI_MSG(name, message)                                                                  \
    (std::string("{ \"contents\": [") +                                                            \
     "{ \"role\": \"user\", \"parts\": [ { \"text\": \"Tu es un BOT IRC qui s'appelle  " + name +  \
     ". Fais des réponses en une seule ligne (tu peux faire plusieurs phrases, mais sans \\n) "    \
     "et sans markdown. Réponds dans la même langue que la question.\" } ] },"                     \
     "{ \"role\": \"user\", \"parts\": [ { \"text\": \"" +                                         \
     message + "\" } ] } ] }")

// Gemini API URL
#define GEMINI_URL(api)                                                                            \
    ("https://generativelanguage.googleapis.com/v1/models/"                                        \
     "gemini-pro:generateContent?key=" +                                                           \
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