#pragma once

// 001
#define RPL_WELCOME(nickname, username, hostname)                                                  \
    (std::string("001 ") + nickname + " :Welcome to the Internet Relay Network " + nickname +      \
     "!" + username + "@" + hostname)

// 002
#define RPL_YOURHOST(nickname, server, version)                                                    \
    (std::string("002 ") + nickname + " :Your host is " + server + ", running version " + version)

// 003
#define RPL_CREATED(nickname, date)                                                                \
    (std::string("003 ") + nickname + std::string(" :This server was created ") + date)

// 004
#define RPL_MYINFO(nickname, servername, version, modes, channels)                                 \
    (std::string("004 ") + nickname + " :" + servername + " " + version + " " + modes + " " +      \
     channels)

// 221
#define RPL_UMODEIS(mode) (std::string("221 ") + mode)

// 301
#define RPL_AWAY(nick, message) (std::string("301 ") + nick + " : " + message)

// 324
#define RPL_CHANNELMODEIS(channel, mode) (std::string("324 ") + channel + " " + mode)

// 331
#define RPL_NOTOPIC(channel) (std::string("331 ") + channel + ":No topic is set")

// 332
#define RPL_TOPIC(channel, topic) (std::string("332 ") + channel + " : " + topic)

// 341
#define RPL_INVITING(channel, nick) (std::string("341 ") + channel + " " + nick)

// 401
#define ERR_NOSUCHNICK(nick) (std::string("401 ") + nick + " :No such nick/channel")

// 403
#define ERR_NOSUCHCHANNEL(nickname, channel)                                                       \
    (std::string("403 ") + nickname + " " + channel + " :No such channel")

// 404
#define ERR_CANNOTSENDTOCHAN(channel) (std::string("404 ") + channel + " :Cannot send to channel")

// 405
#define ERR_TOOMANYCHANNELS(channel)                                                               \
    (std::string("405 ") + channel + " :You have joined too many channels")

// 407
#define ERR_TOOMANYTARGETS(target, errcode, message)                                               \
    (std::string("407 ") + target + " :" + errcode + " recipients. " + message)

// 411
#define ERR_NORECIPIENT(command)                                                                   \
    (std::string("411 ") + std::string("No recipient given (") + command + ")")

// 412
#define ERR_NOTEXTTOSEND(user) (std::string("412 ") + user + " :No text to send")

// 413
#define ERR_NOTOPLEVEL(mask) (std::string("413 ") + mask + " :No toplevel domain specified")

// 414
#define ERR_WILDTOPLEVEL(mask) (std::string("414 ") + mask + " :Wildcard in toplevel domain")

// 431
#define ERR_NONICKNAMEGIVEN (std::string("431") + " :No nickname given")

// 432
#define ERR_ERRONEUSNICKNAME(nick) (std::string("432 ") + nick + (" :Erroneous nickname"))

// 433
#define ERR_NICKNAMEINUSE(nick) (std::string("433 ") + nick + (" :Nickname is already in use"))

// 436
#define ERR_NICKCOLLISION(nick, user, host)                                                        \
    (std::string("436 ") + nick + " :Nickname collision KILL from " + user + "@" + host)

// 437
#define ERR_UNAVAILRESOURCE(nick_channel)                                                          \
    (std::string("437 ") + nick_channel + " :Nick/channel is temporarily unavailable")
// 441
#define ERR_USERNOTINCHANNEL(nick, channel) (std::string("441 ") + nick + " " + channel + " :They aren't on that channel")

// 442
#define ERR_NOTONCHANNEL(channel) (std::string("442 ") + channel + " :You're not on that channel")

// 443
#define ERR_USERONCHANNEL(user, channel)                                                           \
    (std::string("443 ") + user + " " + channel + " :is already on channel")

// 451
#define ERR_NOTREGISTERED(nick) (std::string("451 ") + nick + " :You have not registered")

// 461
#define ERR_NEEDMOREPARAMS(command)                                                                \
    (std::string("461 ") + command + std::string(" :Not enough parameters"))

// 462
#define ERR_ALREADYREGISTRED(user)                                                                 \
    (std::string("462 ") + user + std::string(" :Unauthorized command (already registered)"))

// 464
#define ERR_PASSWDMISMATCH(nick) (std::string("464 ") + nick + " :Password incorect")

// 471
#define ERR_CHANNELISFULL(channel) (std::string("471 ") + channel + " :Cannot join channel (+l)")

// 472
#define ERR_UNKNOWNMODE(mode, nickname)                                                            \
    (std::string("472 ") + nickname + " " + mode + " :is unknown mode char to me")

// 473
#define ERR_INVITEONLYCHAN(channel) (std::string("473 ") + channel + " :Cannot join channel (+i)")

// 474
#define ERR_BANNEDFROMCHAN(channel) (std::string("474 ") + channel + " :Cannot join channel (+b)")

// 475
#define ERR_BADCHANNELKEY(channel) (std::string("475 ") + channel + " :Cannot join channel (+k)")

// 476
#define ERR_BADCHANMASK(channel) (std::string("476 ") + channel + " :Bad Channel Mask")

// 477
#define ERR_NOCHANMODES(channel) (std::string("477 ") + channel + " :Channel doesn't support modes")

// 482
#define ERR_CHANOPRIVSNEEDED(nick, channel)                                                              \
    (std::string("482 ") + nick + " " + channel + " :You're not channel operator")

// 484
#define ERR_RESTRICTED(user) (std::string("484 ") + user + " :You can't kill a server!")

// 501
#define ERR_UMODEUNKNOWNFLAG "501 :Unknown MODE flag"

// 502
#define ERR_USERSDONTMATCH "502 :Cannot change mode for other users"

/*
    pass/user

    461 ERR_NEEDMOREPARAMS
        Not enough params

    462 ERR_ALREADYREGISTRED
        Client already registred

    464 ERR_PASSWDMISMATCH
        Invalid pass

    431 ERR_NONICKNAMEGIVEN
        Missing nick param

    432 ERR_ERRONEUSNICKNAME
        Nick with a forbidden char

    433 ERR_NICKNAMEINUSE
        Nick already exists on server

    436 ERR_NICKCOLLISION
        Nick already exists on connected server

    437 ERR_UNAVAILRESOURCE
        Didnt respect the nick delay

    484 ERR_RESTRICTED
        Si il est restricted r+


    join

    461 ERR_NEEDMOREPARAMS | dup
    474 ERR_BANNEDFROMCHAN
    473 ERR_INVITEONLYCHAN
    475 ERR_BADCHANNELKEY
    478 ERR_CHANNELISFULL
    476 ERR_BADCHANMASK
    403 ERR_NOSUCHCHANNEL
    405 ERR_TOOMANYCHANNELS
    407 ERR_TOOMANYTARGETS
    437 ERR_UNAVAILRESOURCE | dup
    332 RPL_TOPIC


    mode

    461 ERR_NEEDMOREPARAMS | dup
    502 ERR_USERSDONTMATCH
    501 ERR_UMODEUNKNOWNFLAG
    221 RPL_UMODEIS


    topic

    461 ERR_NEEDMOREPARAMS | dup
    442 ERR_NOTONCHANNEL
    331 RPL_NOTOPIC
    332 RPL_TOPIC | dup
    482 ERR_CHANOPRIVSNEEDED
    477 ERR_NOCHANMODES


    kick

    461 ERR_NEEDMOREPARAMS | dup
    403 ERR_NOSUCHCHANNEL | dup
    476 ERR_BADCHANMASK | dup
    482 ERR_CHANOPRIVSNEEDED | dup
    441 ERR_USERNOTINCHANNEL
    442 ERR_NOTONCHANNEL | dup


    invite

    461 ERR_NEEDMOREPARAMS | dup
    442 ERR_NOTONCHANNEL | dup
    401 ERR_NOSUCHNICK
    482 ERR_CHANOPRIVSNEEDED | dup
    443 ERR_USERONCHANNEL
    341 RPL_INVITING
    301 RPL_AWAY


    privmsg

    OK 411 ERR_NORECIPIENT
    OK 412 ERR_NOTEXTTOSEND
    OK 401 ERR_NOSUCHNICK
    OK 401 ERR_NOSUCHCHANNEL
    404 ERR_CANNOTSENDTOCHAN
    413 ERR_NOTOPLEVEL
    414 ERR_WILDTOPLEVEL
    407 ERR_TOOMANYTARGETS

*/
