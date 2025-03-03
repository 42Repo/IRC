#include <Server.h>
#include <csignal>
#include <iostream>
#include <sstream>

static Server *g_server = NULL;

static bool isValidPort(int port) { return port >= 1024 && port <= 65535; }

static bool isValidPassword(const std::string &password) {
    if (password.empty() || password.length() > 128)
        return false;
    return true;
}

static int stringToInt(const std::string &str) {
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

static void handle_sigint(int sig) {
    (void)sig;
    if (g_server) {
        g_server->setShutdownFlag(true);
    }
}

static int setSignal(void) {
    struct sigaction sa;
    sa.sa_handler = handle_sigint;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    if (sigaction(SIGINT, &sa, NULL) == -1) {
        return EXIT_FAILURE;
    }
    return 0;
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

        g_server = &server;
        setSignal();

        server.run();
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
