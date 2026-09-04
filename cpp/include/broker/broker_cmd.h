#ifndef BROKER_BROKER_CMD_H
#define BROKER_BROKER_CMD_H

#include "broker_cmd_type.h"
#include <tag.h>
#include <functional>
#include <map>

class BrokerCmd{
public:
    typedef std::function<BrokerCmdReply(const BrokerCmdMessage& msg)> CallerFnc;

    CallerFnc operator[](const std::string& name);

protected:
    explicit BrokerCmd(const std::map<std::string, CallerFnc>& callers);

private:
    std::map<std::string, CallerFnc> _callers;
};
#endif //BROKER_BROKER_CMD_H
