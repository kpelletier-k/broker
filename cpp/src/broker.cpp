#include "../include/broker.h"


Broker::Broker(const std::map<std::string,std::shared_ptr<Layer>>& layers,
                const std::map<std::string,std::shared_ptr<BrokerRepository>>& repositories,
                const std::map<std::string, BrokerCmd::CallerFnc>& callers,
                const std::map<std::string,std::shared_ptr<Tag>>& tags) :
        BrokerLayers(layers),
        BrokerRepositories(repositories),
        BrokerCmd(callers),
        BrokerTags(tags){
    _publish_attached();
    _publish_registered();
}

Broker::~Broker(){
    _publish_detached();
}

std::shared_ptr<Tag> Broker::tag(const std::string& name){
    return BrokerTags::operator[](name);}

BrokerCmd::CallerFnc Broker::cmd(const std::string& name){
    return BrokerCmd::operator[](name);}

std::shared_ptr<BrokerRepository> Broker::repository(const std::string& name){
    return BrokerRepositories::operator[](name);}

void Broker::on_log(const std::string& name, EventLogType type, const std::string& message){
    int y = 0;
}

