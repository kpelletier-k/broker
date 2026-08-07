#include "../include/broker_repository.h"

void BrokerRepository::registered(){
    _publish_registered();}
void BrokerRepository::unregistered(){
    _publish_unregistered();}