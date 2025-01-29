#pragma once

// 001
#define RPL_WELCOME(nickname, username, hostname)                                                  \
    ("Welcome to the Internet Relay Network " + nickname + "!" + username + "@" + hostname)

// 002
#define RPL_YOURHOST(server, version) ("Your host is " + server + ", running version " + version)

// 003
#define RPL_CREATED(date) (std::string("This server was created ") + date)

// 004
#define RPL_MYINFO(servername, version, modes, channels)                                           \
    (servername + " " + version + " " + modes + " " + channels)

// 221
#define RPL_UMODEIS(mode) (mode)

// 301
#define RPL_AWAY(nick, message) (nick + " : " + message)

// 331
#define RPL_NOTOPIC(channel) (channel + ":No topic is set")

// 332
#define RPL_TOPIC(channel, topic) (channel + " : " + topic)

// 341
#define RPL_INVITING(channel, nick) (channel + " " + nick)

// 403
#define ERR_NOSUCHCHANNEL(channel) (channel + " :No such channel")

// 404
#define ERR_CANNOTSENDTOCHAN(channel) (channel + " :Cannot send to channel")

// 405
#define ERR_TOOMANYCHANNELS(channel) (channel + " :You have joined too many channels")

// 407
#define ERR_TOOMANYTARGETS(target, errcode, message)                                               \
    (target + " :" + errcode + " recipients. " + message)

// 411
#define ERR_NORECIPIENT(command) ("No recipient given (" + command + ")")

// 412
#define ERR_NOTEXTTOSEND (":No text to send")

// 413
#define ERR_NOTOPLEVEL(mask) (mask + " :No toplevel domain specified")

// 414
#define ERR_WILDTOPLEVEL(mask) (mask + " :Wildcard in toplevel domain")

// 431
#define ERR_NONICKNAMEGIVEN ":No nickname given"

// 432
#define ERR_ERRONEUSNICKNAME(nick) (nick + (" :Erroneous nickname"))

// 433
#define ERR_NICKNAMEINUSE(nick) (nick + (" :Nickname is already in use"))

// 436
#define ERR_NICKCOLLISION(nick, user, host)                                                        \
    (nick + " :Nickname collision KILL from " + user + "@" + host)

// 437
#define ERR_UNAVAILRESOURCE(nick_channel)                                                          \
    (nick_channel + " :Nick/channel is temporarily unavailable")

// 442
#define ERR_NOTONCHANNEL(channel) (channel + " :You're not on that channel")

// 443
#define ERR_USERONCHANNEL(user, channel) (user + " " + channel + " :is already on channel")

// 461
#define ERR_NEEDMOREPARAMS(command) (command + " :Not enough parameters")

// 462
#define ERR_ALREADYREGISTRED ": Unauthorized command (already registered)"

// 471
#define ERR_CHANNELISFULL(channel) (channel + " :Cannot join channel (+l)")

// 473
#define ERR_INVITEONLYCHAN(channel) (channel + " :Cannot join channel (+i)")

// 474
#define ERR_BANNEDFROMCHAN(channel) (channel + " :Cannot join channel (+b)")

// 475
#define ERR_BADCHANNELKEY(channel) (channel + " :Cannot join channel (+k)")

// 476
#define ERR_BADCHANMASK(channel) (channel + " :Bad Channel Mask")

// 477
#define ERR_NOCHANMODES(channel) (channel + " :Channel doesn't support modes")

// 482
#define ERR_CHANOPRIVSNEEDED(channel) (channel + " :You're not channel operator")

// 484
#define ERR_RESTRICTED ":You can't kill a server!"

// 501
#define ERR_UMODEUNKNOWNFLAG ":Unknown MODE flag"

// 502
#define ERR_USERSDONTMATCH ":Cannot change mode for other users"

/*
    pass/user

    461 ERR_NEEDMOREPARAMS
        Not enough params

    462 ERR_ALREADYREGISTRED
        Client already registred

    nick

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

    411 ERR_NORECIPIENT
    412 ERR_NOTEXTTOSEND
    404 ERR_CANNOTSENDTOCHAN
    413 ERR_NOTOPLEVEL
    414 ERR_WILDTOPLEVEL
    407 ERR_TOOMANYTARGETS
    401 ERR_NOSUCHNICK | dup
    301 RPL_AWAY | dup

*/
