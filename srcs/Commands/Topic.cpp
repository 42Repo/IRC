#include "../../includes/CommandHandler.h"
#include "../../includes/Server.h"
#include <ostream>

// TODO - Command - TOPIC
void CommandHandler::handleTopic(Client *client, const std::vector<std::string> &input) {
    std::cout << client->getNickname() << " called TOPIC" << std::endl;
    (void)input;
    Server  *server = client->getServer();
    Channel *channel = server->getChannelByName(input[2]);
    std::cout << "[" << input[3] << "]" << std::endl;
    if (input[3].length() > 1) {
        channel->setTopic(input[3]);
        client->sendMessage(std::string(":") + client->getNickname() + "!" + client->getUsername() +
                            "@" + client->getHostname() + " TOPIC " + input[2] + " :" +
                            channel->getTopic() + "\r\n");
    } else
        client->sendNumericReply("332",
                                 RPL_TOPIC(client->getNickname(), input[2], channel->getTopic()));
    //: user42!~user@host TOPIC #gaming :Nouvelle discussion sur les jeux rétro !
}
// :irc.example.com 332 user42 #gaming :Bienvenue sur le canal de discussion !