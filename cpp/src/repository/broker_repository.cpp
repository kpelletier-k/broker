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

