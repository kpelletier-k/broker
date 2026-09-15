#ifndef BROKER_BROKER_CMD_H
#define BROKER_BROKER_CMD_H

#include <functional>
#include <map>
#include <string>
#include <mutex>

#include "../layers/layer.h"

class BrokerCmd{
public:
    typedef std::function<bool(const std::shared_ptr<BrokerSession>& session, const ProtoMessage& msg, const LayerReplyFnc& reply_fnc)> CmdCallFnc;

    ~BrokerCmd();

    void emplace_cmd(const std::string& cmd_name, const CmdCallFnc& fnc);
    CmdCallFnc find_cmd(const std::string& cmd_name);
private:
    std::mutex _mtx;
    std::map<std::string, CmdCallFnc> _callers;
};
#endif //BROKER_BROKER_CMD_H
