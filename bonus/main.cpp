#include "Bot.h"
#include <csignal>
#include <sstream>

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

void handle_sigint(int sig) {
    (void)sig;
    std::cout << "omg sig" << std::endl;
}

static int setSignal(void) {
    struct sigaction sa;
    sa.sa_handler = handle_sigint; // Définition du handler
    sigemptyset(&sa.sa_mask);      // Pas de masquage de signaux supplémentaires
    sa.sa_flags = 0;               // Options par défaut

    // Appliquer la gestion du signal SIGINT
    if (sigaction(SIGINT, &sa, NULL) == -1) {
        // std::/("sigaction");
        return EXIT_FAILURE;
    }
    return 0;
}

int main(int argc, char **argv) {
    if (argc != 6) {
        std::cout << "Usage: ./bot [adress] [port] [password] [name] [Gemini API Key]" << std::endl;
        return 1;
    }

    try {
        int port = stringToInt(argv[2]);
        if (!isValidPort(port)) {
            std::ostringstream oss;
            oss << port;
            throw std::runtime_error("Invalid port number: " + oss.str() +
                                     ". Port must be between 1024 and 65535.");
        }

        std::string password = argv[3];
        if (!isValidPassword(password)) {
            throw std::runtime_error("Invalid password. Password cannot be empty and must be no "
                                     "longer than 128 characters.");
        }

        Bot bot(argv[4], stringToInt(argv[2]), argv[1], argv[3], argv[5]);
        bot.joinServer();
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    setSignal();

    return 0;
}
