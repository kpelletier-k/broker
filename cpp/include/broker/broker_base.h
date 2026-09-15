#ifndef BROKER_BROKER_BASE_H
#define BROKER_BROKER_BASE_H

#include <functional>
#include <map>
#include <string>
#include <mutex>

#include "../layers/layer.h"

class BrokerBase{

public:
    typedef std::function<void(const ProtoMessage& msg, const LayerReplyFnc& reply_fnc)> CmdCallFnc;

    ~BrokerBase();

    void emplace(const std::string& cmd_name, const CmdCallFnc& fnc);
    CmdCallFnc find(const std::string& cmd_name);
private:
    std::mutex _mtx;
    std::map<std::string, CmdCallFnc> _callers;
};
#endif //BROKER_BROKER_BASE_H
