// 001    RPL_WELCOME
//            "Welcome to the Internet Relay Network
//             <nick>!<user>@<host>"
//     002    RPL_YOURHOST
//            "Your host is <servername>, running version <ver>"
//     003    RPL_CREATED
//            "This server was created <date>"
//     004    RPL_MYINFO
//            "<servername> <version> <available user modes>
//             <available channel modes>"

#pragma once

// 001
#define RPL_WELCOME(nickname, username, hostname)                                                  \
    (std::string("Welcome to the Internet Relay Network ") + nickname + "!" + username + "@" +     \
     hostname)
// 002
#define RPL_YOURHOST(server, version)                                                              \
    (std::string("Your host is ") + server + std::string(", running version ") + vers)
// 003
#define RPL_CREATED(date) (std::string("This server was created ") + date)
// 004
#define RPL_MYINFO(servername, version, modes, channels)                                           \
    (servername + " " + version + " " + modes + " " + channels)


/*
    403 ERR_NOSUCHCHANNEL
    404 ERR_CANNOTSENDTOCHAN
    405 ERR_TOOMANYCHANNELS
    407 ERR_TOOMANYTARGETS
    411 ERR_NORECIPIENT
    412 ERR_NOTEXTTOSEND
    413 ERR_NOTOPLEVEL
    414 ERR_WILDTOPLEVEL
    431 ERR_NONICKNAMEGIVEN 
    432 ERR_ERRONEUSNICKNAME
    433 ERR_NICKNAMEINUSE
    436 ERR_NICKCOLLISION
    437 ERR_UNAVAILRESOURCE
    442 ERR_NOTONCHANNEL
    443 ERR_USERONCHANNEL
    461 ERR_NEEDMOREPARAMS
    462 ERR_ALREADYREGISTRED
    473 ERR_INVITEONLYCHAN
    474 ERR_BANNEDFROMCHAN
    475 ERR_BADCHANNELKEY
    476 ERR_BADCHANMASK
    477 ERR_NOCHANMODES
    478 ERR_CHANNELISFULL
    482 ERR_CHANOPRIVSNEEDED
    484 ERR_RESTRICTED
    
    221 RPL_UMODEIS
    
    301 RPL_AWAY
    331 RPL_NOTOPIC
    332 RPL_TOPIC
    341 RPL_INVITING
    
    501 ERR_UMODEUNKNOWNFLAG
    502 ERR_USERSDONTMATCH
*/


/*
    pass/user

    461 ERR_NEEDMOREPARAMS
    462 ERR_ALREADYREGISTRED


    nick

    431 ERR_NONICKNAMEGIVEN 
    432 ERR_ERRONEUSNICKNAME
    433 ERR_NICKNAMEINUSE
    436 ERR_NICKCOLLISION
    437 ERR_UNAVAILRESOURCE
    484 ERR_RESTRICTED


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