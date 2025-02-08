#include <exception>
#include <string>
#include "Client.h"

class Error
{
	Error(std::string errMessage, Client *client);
	~Error();
	class IRCError : public std::exception{
		virtual const char* what() const throw();
	};

	private:
		Server* _server;
};