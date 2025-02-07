#include "../../includes/CommandHandler.h"
#include "../../includes/Server.h"

// TODO - Command - QUIT
void CommandHandler::handleQuit(Client *client, const std::vector<std::string> &input) {
    std::cout << client->getNickname() << " called QUIT" << std::endl;
    (void)input;
}
