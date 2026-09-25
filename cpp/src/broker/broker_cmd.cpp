#include "../../include/broker/broker_cmd.h"

BrokerCmd::BrokerCmd(const std::map<std::string, CmdCallFnc>& callers) : _callers(callers){}

BrokerCmd::~BrokerCmd(){
    _mtx.lock();
    _callers.clear();
    _mtx.unlock();
}

void BrokerCmd::emplace_cmd(const std::string& cmd_name, const CmdCallFnc& fnc){
    std::lock_guard<std::mutex> lock(_mtx);
    _callers[cmd_name] = fnc;
}

BrokerCmd::CmdCallFnc BrokerCmd::find_cmd(const std::string& cmd_name){
    std::lock_guard<std::mutex> lock(_mtx);
    const auto it = _callers.find(cmd_name);
    return it != _callers.end() ? it->second : nullptr;
}