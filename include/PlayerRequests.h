#ifndef PLAYERREQUESTS_H
#define PLAYERREQUESTS_H

#include <string>

struct PlayerRequest {
    std::string message;
};

struct PlayerReply {
    bool answer;
};

#endif //PLAYERREQUESTS_H
