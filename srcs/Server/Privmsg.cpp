#include "../../includes/CommandHandler.h"
#include "../../includes/Messages.h"
#include "../../includes/Server.h"
#include <string>


// TODO - Command - PRIVMSG
void CommandHandler::handlePrivmsg(Client *client, const std::vector<std::string> &input) {
    std::cout << client->getNickname() << " called PRIVATEMSG" << std::endl;
    (void)input;
}