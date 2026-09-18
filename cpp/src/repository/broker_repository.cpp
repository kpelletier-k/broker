#include "../../include/repository/broker_repository.h"
#include <console.h>

void BrokerRepository::registered(){
    _registered();
    Console::info(name + ": registered");
}

void BrokerRepository::unregistered(){
    _unregistered();
    Console::info(name + ": unregistered");
}

void BrokerRepository::session_open(const std::shared_ptr<BrokerSession>& session){
    _session_open(session);
}

void BrokerRepository::session_close(const std::shared_ptr<BrokerSession>& session){
    _session_close(session);
}