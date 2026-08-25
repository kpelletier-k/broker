#include "../include/broker_repository.h"

void BrokerRepository::registered(){
    on_registered();
    _publish_registered();}
void BrokerRepository::unregistered(){
    on_unregistered();
    _publish_unregistered();}