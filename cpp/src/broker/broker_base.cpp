#include "../../include/broker/broker_base.h"

BrokerBase::~BrokerBase(){
    _mtx.lock();
    _callers.clear();
    _mtx.unlock();
}

void BrokerBase::emplace(const std::string& cmd_name, const CmdCallFnc& fnc){
    std::lock_guard<std::mutex> lock(_mtx);
    _callers[cmd_name] = fnc;
}

BrokerBase::CmdCallFnc BrokerBase::find(const std::string& cmd_name){
    std::lock_guard<std::mutex> lock(_mtx);
    const auto it = _callers.find(cmd_name);
    return it != _callers.end() ? it->second : nullptr;
}