#include "../../include/broker/broker_cmd.h"

BrokerCmd::BrokerCmd(const std::map<std::string, CallerFnc>& callers):
    _callers(callers){}

BrokerCmd::CallerFnc BrokerCmd::operator[](const std::string& name){
    const auto it = _callers.find(name);
    return it != _callers.end() ? it->second : nullptr;
}
