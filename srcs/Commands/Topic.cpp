#include "../../includes/CommandHandler.h"
#include "../../includes/Server.h"

// TODO - Command - TOPIC
void CommandHandler::handleTopic(Client *client, const std::vector<std::string> &input) {
    std::cout << client->getNickname() << " called TOPIC" << std::endl;
    (void)input;
}