#include "Bot.h"
#include <vector>

static std::vector<std::string> commandParser(std::string input) {
    std::vector<std::string> command(4);

    std::istringstream stream(input);

    if (input[0] == ':') {
        std::getline(stream, command[0], ' ');
        command[0] = command[0].substr(1);
    }

    stream >> command[1];

    std::getline(stream >> std::ws, command[2], ':');
    if (!command[2].empty() && command[2][command[2].size() - 1] == ' ') {
        command[2].erase(command[2].size() - 1, 1);
    }
    std::getline(stream, command[3]);

    if (!command[2].empty() && command[2][command[2].length() - 1] == '\n') {
        command[2].erase(command[2].length() - 1);
    }
    return command;
}

void Bot::sendMessage(const std::string &msg) {
    std::string sentMsg = msg + "\r\n";
    ssize_t     bytesSent = send(_fd, sentMsg.c_str(), sentMsg.length(), 0);
    if (bytesSent == -1) {
        // Handle error (e.g., log it, close the connection)
        // TODO: Implement error handling
    }
}

Bot::Bot(std::string name, int port, std::string servAdress, std::string password,
         std::string api_key)
    : _name(name),
      _port(port),
      _servAddress(servAdress),
      _password(password),
      _api_key(api_key) {}

Bot::~Bot() {}

static std::string parseGeminiAnswer(std::string answer) {
    std::string        parsedAnswer;
    std::istringstream iss(answer);
    std::string        line;
    while (std::getline(iss, line)) {
        if (line.find("text") != std::string::npos) {
            parsedAnswer = line.substr(10, line.length() - 11);
            parsedAnswer.erase(0, 11);
            break;
        }
    }
    if (parsedAnswer.empty())
        return "I'm drinking a coffee ☕, try again later !";
    return parsedAnswer;
}

void Bot::joinServer() {

    std::cout << "Server adress : " << _servAddress << std::endl;
    std::cout << "Port : " << _port << std::endl;
    std::cout << "Password : " << _password << std::endl;
    std::cout << "Name : " << _name << std::endl;

    if ((_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        throw std::runtime_error("Failed to create socket");

    sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(static_cast<uint16_t>(_port));

    if (inet_pton(AF_INET, _servAddress.c_str(), &address.sin_addr) <= 0)
        throw std::runtime_error("Invalid server address");

    if (connect(_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
        throw std::runtime_error("Failed to connect to server");

    _pollfd.fd = _fd;
    _pollfd.events = POLLIN;
    _pollfd.revents = 0;
    loginToServ();
}

void Bot::loginToServ() {

    std::cout << "PASS " + _password << std::endl;
    sendMessage("PASS " + _password);
    sendMessage("NICK " + _name);
    sendMessage("USER " + _name + " 0 * :bot");

    char buffer[1024];
    while (true) {
        std::memset(buffer, 0, sizeof(buffer));
        ssize_t bytesRead = recv(_fd, buffer, sizeof(buffer) - 1, 0);

        if (bytesRead > 0) {
            buffer[bytesRead] = '\0'; // Assurer une fin de chaîne correcte
            answer(commandParser(buffer));
            std::cout << buffer << std::endl;
        } else if (bytesRead == 0) {
            std::cout << "Server closed connexion" << std::endl;
            break;
        } else {
            std::cout << "Error" << std::endl;
            break;
        }
    }
}

static bool startsWith(const std::string &str, const std::string &prefix) {
    return str.compare(0, prefix.length(), prefix) == 0;
}

static bool endsWith(const std::string &str, const std::string &suffix) {
    return str.compare(str.length() - suffix.length(), suffix.length(), suffix) == 0;
}

size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    ((std::string *)userp)->append((char *)contents, size * nmemb);
    return size * nmemb;
}

std::string Bot::getLLMReply(std::string message) {
    CURL       *curl;
    CURLcode    res;
    std::string readBuffer;
    std::string jsonData = GEMINI_MSG(_name, message);
    curl = curl_easy_init();
    if (curl) {
        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/json");

        curl_easy_setopt(curl, CURLOPT_URL, GEMINI_URL(_api_key).c_str());
        curl_easy_setopt(curl, CURLOPT_POST, 1L);
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonData.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        curl_slist_free_all(headers);
        if (res != CURLE_OK)
            return NULL;
    }
    return readBuffer;
}

bool Bot::isAlreadyJoined(std::string channel) {
    for (std::vector<std::string>::iterator it = _channels.begin(); it != _channels.end(); ++it) {
        if (*it == channel)
            return true;
    }
    return false;
}

static std::vector<std::string> splitAnswer(std::string answer)
{
    std::vector<std::string> answerSplited;
    std::string temp;
    
    for (size_t i = 0; i < answer.size(); ++i)
    {
        if (i < answer.size() - 1 && answer[i] == '\\' && answer[i + 1] == 'n')
        {
            answerSplited.push_back(temp);
            temp.clear();
            ++i; // Sauter le 'n' car on a déjà traité "\n"
        }
        else
        {
            temp += answer[i];
        }
    }
    
    answerSplited.push_back(temp); // Ajouter le dernier segment
    return answerSplited;
}
void Bot::answer(std::vector<std::string> message) {

    if (message[3].length() > 0 && endsWith(message[3], "\r"))
        message[3].erase(message[3].length() - 1, 1);

    if (message[1] == "INVITE") {
        if (isAlreadyJoined(message[3])) {
            sendMessage("PRIVMSG " + message[3] + " :Im already here !");
            return;
        }
        sendMessage("JOIN " + message[3]);
        sendMessage("PRIVMSG " + message[3] + " :Hey, I'm " + _name +
                    "! Send [!tellme] and ask me anything!");
        _channels.push_back(message[3]);
    }
    if (message[1] == "JOIN") {
        sendMessage("MODE " + message[3]);
        sendMessage("WHO " + message[3]);
    } else if (message[1] == "PRIVMSG" && message[3].length() > 0 &&
               startsWith(message[3], "!tellme")) {
        std::string answer = parseGeminiAnswer(getLLMReply(message[3]));
        std::vector<std::string> answerSplited = splitAnswer(answer);

        for(std::vector<std::string>::iterator it = answerSplited.begin(); it != answerSplited.end(); ++it)
        {

            sendMessage("PRIVMSG " + message[2] + " :" + *it);
        }
    }
}
