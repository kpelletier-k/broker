#include "../include/broker.h"

Broker::Broker(const std::initializer_list<std::shared_ptr<Layer>>& layers, const std::initializer_list<std::shared_ptr<BrokerRepository>>& repositories):
    Broker(_from(layers),_from(repositories)){}

Broker::Broker(const std::map<std::string,std::shared_ptr<Layer>>& layers, const std::map<std::string,std::shared_ptr<BrokerRepository>>& repositories) :
    _layers(layers), _repositories(repositories){
    _publish_attached(_layers);
    _publish_registered(_repositories);
}

Broker::~Broker(){
    _publish_detached(_layers);
}

void Broker::emplace(const std::shared_ptr<BrokerRepository>& repository){
    if (!repository || repository->name.empty())return;
    std::lock_guard<std::mutex> lock(_repositories_mtx);
    _repositories.emplace(repository->name, repository);
    repository->registered();
}

void Broker::remove(const std::shared_ptr<BrokerRepository>& repository){
    if (!repository || repository->name.empty())return;
    std::lock_guard<std::mutex> lock(_repositories_mtx);
    _repositories.erase(repository->name);
    repository->unregistered();
}

void Broker::_publish_attached(const std::map<std::string, std::shared_ptr<Layer>>& layers){
    for (auto& layer : layers)
        if (layer.second && !layer.first.empty())
            layer.second->attached();
}

void Broker::_publish_detached(const std::map<std::string, std::shared_ptr<Layer>>& layers){
    for (auto& layer : layers)
        if (layer.second && !layer.first.empty())
            layer.second->detached();
}

void Broker::_publish_registered(const std::map<std::string, std::shared_ptr<BrokerRepository>>& repositories){
    for (auto& repo : repositories)
        if (repo.second && !repo.first.empty())
            repo.second->registered();
}

template<typename T>
std::map<std::string, std::shared_ptr<T>> Broker::_from(const std::initializer_list<std::shared_ptr<T>>& values){
    std::map<std::string,std::shared_ptr<T>> results;
    for (auto& e : values)
        results.emplace(e->name, e);
    return results;
}
