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

// 315
#define RPL_ENDOFWHO(nick, mask) (std::string("315 ") + nick + " " + mask + " :End of /WHO list.")

// 321
#define RPL_LISTSTART(nick) (std::string("321 " + nick + " Channel Users Topic"))

// 322
#define RPL_LIST(nick, channel, num, topic)                                                        \
    (std::string("322 " + nick + " " + channel + " " + num + " :" + topic))

// 323
#define RPL_LISTEND(nick) (std::string("323 " + nick + " :End of /LIST"))

// 324
#define RPL_CHANNELMODEIS(channel, mode) (std::string("324 ") + channel + " " + mode)

// 331
#define RPL_NOTOPIC(user, channel) (std::string("331 ") + user + " " + channel + " :No topic is set")

// 332
#define RPL_TOPIC(nick, channel, topic) (std::string("332 ") + nick + " " + channel + " :" + topic)

// 333
#define RPL_TOPICTIME(nick, channel, admin, timestamp)                                             \
    (std::string("333 ") + nick + " " + channel + " " + admin + " " + timestamp + " :test")

// 341
#define RPL_INVITING(nick, channel, target)                                                        \
    (std::string("341 ") + nick + " " + target + " :" + channel)

// 352
#define RPL_WHOREPLY(nick, channel, username, hostname, servername, nickname, flags, hopcount,     \
                     realname)                                                                     \
    (std::string("352 ") + nick + " " + channel + " " + username + " " + hostname + " " +          \
     servername + " " + nickname + " " + flags + " " + hopcount + " " + realname)

// 372
#define RPL_MOTD(nick) (std::string("372 ") + nick + " :made by asuc & mbuchs")

// 375
#define RPL_MOTDSTART(nick) (std::string("375 ") + nick + " :Have a nice correction :)")

// 376
#define RPL_MOTDEND(nick) (std::string("376 ") + nick + " :pls give us 125")

// 401
#define ERR_NOSUCHNICK(nick, target)                                                               \
    (std::string("401 ") + nick + " " + target + " :No such nick/channel")

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
#define ERR_NICKNAMEINUSE(nick) (std::string("433 ") + (":Nickname ") + nick + (" is already in use"))

// 436
#define ERR_NICKCOLLISION(nick, user, host)                                                        \
    (std::string("436 ") + nick + " :Nickname collision KILL from " + user + "@" + host)

// 437
#define ERR_UNAVAILRESOURCE(nick_channel)                                                          \
    (std::string("437 ") + nick_channel + " :Nick/channel is temporarily unavailable")
// 441
#define ERR_USERNOTINCHANNEL(nick, channel)                                                        \
    (std::string("441 ") + nick + " " + channel + " :They aren't on that channel")

// 442
#define ERR_NOTONCHANNEL(nick, channel)                                                            \
    (std::string("442 ") + nick + " " + channel + " :You're not on that channel")

// 443
#define ERR_USERONCHANNEL(user, target, channel)                                                   \
    (std::string("443 ") + user + " " + target + " " + channel + " :is already on channel")

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
#define ERR_CHANOPRIVSNEEDED(nick, channel)                                                        \
    (std::string("482 ") + nick + " " + channel + " :You're not channel operator")

// 484
#define ERR_RESTRICTED(user) (std::string("484 ") + user + " :You can't kill a server!")

// 501
#define ERR_UMODEUNKNOWNFLAG "501 :Unknown MODE flag"

// 502
#define ERR_USERSDONTMATCH "502 :Cannot change mode for other users"
