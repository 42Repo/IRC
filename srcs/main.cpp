#include "../includes/Server.h"
#include <arpa/inet.h>
#include <cstdlib>

int main(int argc, char **argv) {
    if (argc != 3) {
        std::cout << "Error: Invalid argument number" << std::endl;
        std::cout << "Usage: ./ircserv [port] [password]" << std::endl;
        exit(1);
    }
    std::cout << "Port = " << argv[1] << std::endl;
    std::cout << "Password = " << argv[2] << std::endl;
}
