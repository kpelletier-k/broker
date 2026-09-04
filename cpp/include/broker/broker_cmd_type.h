#ifndef BROKER_BROKER_CMD_TYPE_H
#define BROKER_BROKER_CMD_TYPE_H

#include <variant>
#include <tag.h>

struct BrokerCmdNotif{
    Tag args;
};

struct BrokerCmdMethod{
    std::string id;
    Tag args;
};

struct BrokerCmdResult{
    std::string id;
    Tag args;
};

struct BrokerCmdError{
    std::string id;
    std::string message;
};

typedef std::variant<std::monostate, BrokerCmdResult, BrokerCmdError> BrokerCmdReply;
typedef std::variant<std::monostate, BrokerCmdNotif, BrokerCmdMethod> BrokerCmdMessage;


#endif //BROKER_BROKER_CMD_TYPE_H
