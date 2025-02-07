#include "../../includes/CommandHandler.h"
#include "../../includes/Server.h"


// TODO - Command - INVITE
void CommandHandler::handleInvite(Client *client, const std::vector<std::string> &input) {
    std::cout << client->getNickname() << " called INVITE" << std::endl;
    (void)input;
}