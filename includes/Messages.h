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

// 001

#pragma once

#define RPL_WELCOME(nickname, username, hostname)                                                  \
    (std::string("Welcome to the Internet Relay Network ") + nickname + "!" + username + "@" +      \
     hostname)
