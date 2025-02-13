#pragma once

#include "Client.h"
#include <exception>
#include <string>

class Error {
  public:
    Error(std::string errMessage, Client *client);
    ~Error() {};
    class IRCError : public std::exception {
      public:
        IRCError(const char *msg);
        virtual ~IRCError() throw();
        virtual const char *what() const throw();

      private:
        char *_msg;
    };

  private:
    Server *_server;
};
