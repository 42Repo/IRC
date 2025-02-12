#include "../includes/Server.h"
#include <iostream>
#include <sstream>

// TODO - mode du serveur (??)
// TODO - check perms
// TODO - motd file
// TODO - fix memory leaks
// TODO - create a bot

bool isValidPort(int port) { return port >= 1024 && port <= 65535; }

bool isValidPassword(const std::string &password) {
    if (password.empty()) {
        return false;
    }
    if (password.length() > 128) {
        return false;
    }

    return true;
}

int stringToInt(const std::string &str) {
    std::istringstream iss(str);
    int                num;
    if (!(iss >> num)) {
        throw std::runtime_error("Invalid integer: " + str);
    }
    char remaining;
    if (iss >> remaining) {
        throw std::runtime_error("Invalid integer: " + str);
    }
    return num;
}

int main(int argc, char **argv) {
    if (argc != 3) {
        std::cout << "Usage: ./ircserv [port] [password]" << std::endl;
        return 1;
    }

    try {
        int port = stringToInt(argv[1]);
        if (!isValidPort(port)) {
            std::ostringstream oss;
            oss << port;
            throw std::runtime_error("Invalid port number: " + oss.str() +
                                     ". Port must be between 1024 and 65535.");
        }

        std::string password = argv[2];
        if (!isValidPassword(password)) {
            throw std::runtime_error("Invalid password. Password cannot be empty and must be no "
                                     "longer than 128 characters.");
        }

        Server server(port, password);
        server.run();
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
