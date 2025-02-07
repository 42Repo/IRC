#include "../../includes/CommandHandler.h"
#include "../../includes/Server.h"

// TODO - Command - MODE
void CommandHandler::handleMode(Client *client, const std::vector<std::string> &input) {
    std::cout << client->getNickname() << " called MODE" << std::endl;
    (void)input;
}