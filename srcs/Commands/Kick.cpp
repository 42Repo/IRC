#include "../../includes/CommandHandler.h"
#include "../../includes/Server.h"

// TODO - Command - KICK
void CommandHandler::handleKick(Client *client, const std::vector<std::string> &input) {
    std::cout << client->getNickname() << " called KICK" << std::endl;
    (void)input;
}