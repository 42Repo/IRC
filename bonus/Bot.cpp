#include "Bot.h"
#include <arpa/inet.h>
#include <cstring>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>
#include <sstream>

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
			answer(commandParser(buffer));
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
//TODO - manage channel join (on /invite)


void Bot::answer(std::vector<std::string> message)
{
	if(message[3].length() > 0)
		message[3].erase(message[3].length() -1, 1);
	// std::cout << "bah [" << message[3] <<  "]"<< std::endl;

	// for(unsigned long i = 0; i < message[3].length(); i++)
	// {
	// 	std::cout << message[3][i] << std::endl;
	// }
	if (message[1] == "INVITE")
	{
		sendMessage("JOIN " + message[3]);
		
	}
	if(message[1] == "JOIN"){

		sendMessage("MODE " + message[3]);
		sendMessage("WHO " + message[3]);
	}
	else if(message[1] == "PRIVMSG")
	{
		sendMessage("PRIVMSG " + message[2] + " :BERNARDO IS HERE NIARK NIARK");
	}


		// sendMessage("PRIVMSG " + message[3] + " :BERNARDO IS HERE NIARK NIARK");
}

void Bot::setName(std::string name) { _name = name; }

std::string Bot::getName() { return _name; }