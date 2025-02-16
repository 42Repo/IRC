#include "Bot.h"
#include <arpa/inet.h>
#include <cstring>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>

void Bot::sendMessage(const std::string &msg) {
	std::string sentMsg = msg + "\r\n";
    ssize_t bytesSent = send(_fd, sentMsg.c_str(), sentMsg.length(), 0);
    if (bytesSent == -1) {
        // Handle error (e.g., log it, close the connection)
        // TODO: Implement error handling
    }
}

Bot::Bot(std::string name, int port, std::string servAdress, std::string password)
    : _name(name),
      _port(port),
      _servAddress(servAdress),
      _password(password) {}

Bot::~Bot() {}

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

	std::cout << "PASS " + _password <<std::endl;
	sendMessage("PASS " + _password);
	sendMessage("NICK " + _name);
	sendMessage("USER " + _name + " 0 * :bot");
	
	char buffer[1024];
	while (true) {
		std::memset(buffer, 0, sizeof(buffer));
		ssize_t bytesRead = recv(_fd, buffer, sizeof(buffer) - 1, 0);

		if (bytesRead > 0) {
			buffer[bytesRead] = '\0'; // Assurer une fin de chaîne correcte
			answer(buffer);
			std::cout << buffer << std::endl;
		} else if (bytesRead == 0) {
			std::cout << "Server closed connexion" << std::endl;
			break;
		} else {
			std::cout << "Error"  << std::endl;
			break;
		}
	}
	
}


//TODO - splitServerMessage
//TODO - send privmsg to user
//TODO - manage channel join

// std::vector<std::string> splitServerMessage(std::string message){	
// 	std::vector<std::string> split;
// 	for(size_t i = 0; i < message.size(); ++i) {
// 		if (message[i] == ' ') {
// 			split.push_back(message.substr(0, i));
// 			message = message.substr(i + 1);
// 			i = 0;
// 		}
// 	}
// 	return split;

void Bot::answer(std::string message)
{
	if (message.find("PING") != std::string::npos)
	{
		std::string pong = " PRIVMSG bernard_ :PONG";
		sendMessage(pong);
	}
}

void Bot::setName(std::string name) { _name = name; }

std::string Bot::getName() { return _name; }