#include "../../includes/CommandHandler.h"
#include "../../includes/Error.h"
#include "../../includes/Server.h"
#include <fnmatch.h> // For wildcard matching

// static bool matchesMask(const std::string &str, const std::string &mask) {
//     return fnmatch(mask.c_str(), str.c_str(), 0) == 0;
// }

void CommandHandler::handleWho(Client *client, const std::vector<std::string> &input) {

    (void)input;
    (void)client;
}
