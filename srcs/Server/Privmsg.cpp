#include "../../includes/CommandHandler.h"
#include "../../includes/Messages.h"
#include "../../includes/Server.h"
#include <string>
#include <vector>



static void sendPrivUserMsg(Server *server, Client *send, std::string receive, const std::string &message)
{
	std::vector<Client *> clients = server->getClients();
    for (size_t i = 0; i < clients.size(); ++i) {
        if (clients[i]->getNickname() == receive) {
			std::cout << "message envoyé a " <<  clients[i]->getNickname()<< std::endl;
			clients[i]->sendMessage(":" + send->getNickname() + " PRIVMSG " + clients[i]->getNickname() + " :"+ message + "\n\r");
			return ;
        }
    }
	send->sendNumericReply("401", ERR_NOSUCHNICK(receive));
	std::cout << "WRONG NICKNAME CHEF" << std::endl;
}

static void sendPrivChannelMsg(Server *server, Client *send, std::string channelName,const std::string &message)
{
	std::cout << send->getNickname() << " sent [" << message << "] to " << channelName <<std::endl;
	Channel *channel =  server->getChannelByName(channelName);
	std::map<Client*, bool> clients = channel->getMembers();
	for (std::map<Client*, bool>::iterator it = clients.begin(); it != clients.end(); ++it) {
		if(it->first->getNickname() != send->getNickname())
			it->first->sendMessage(":" + send->getNickname() + " PRIVMSG " + channelName + " :"+ message + "\n\r");
    }
	
	(void) server;
}

// TODO - Command - PRIVMSG
void CommandHandler::handlePrivmsg(Client *client, const std::vector<std::string> &input) {
    std::cout << client->getNickname() << " called PRIVATEMSG" << std::endl;
	std::cout << input[2] << std::endl;
	if(input[2].size() == 0)
	{
		client->sendNumericReply("411", ERR_NORECIPIENT("PRIVMSG"));
		return ;
	}
	if(input[3].size() == 0)
	{
		client->sendNumericReply("412", ERR_NOTEXTTOSEND(client->getNickname()));
		return ;
	}
	if(input[2][0] == '#')
		sendPrivChannelMsg(_server, client, input[2],input[3]);
	else
	 	sendPrivUserMsg(_server, client, input[2], input[3]);

	
	/*
		client : PRIVMSG nickname :message
		server : :prefix PRIVMSG login :message

		client -> server -> client2

	*/

}