#include "../include/broker.h"

Broker::Broker(const std::initializer_list<std::shared_ptr<Layer>>& layers, const std::initializer_list<std::shared_ptr<BrokerRepository>>& repositories):
    Broker(_from(layers),_from(repositories)){}

Broker::Broker(const std::map<std::string,std::shared_ptr<Layer>>& layers, const std::map<std::string,std::shared_ptr<BrokerRepository>>& repositories) :
    _layers(layers), _repositories(repositories){
    for (auto& repo : _repositories)
        if (repo.second && !repo.first.empty())
            repo.second->registered();
}

void Broker::emplace(const std::shared_ptr<BrokerRepository>& repository){
    if (!repository || repository->name.empty())return;
    std::lock_guard<std::mutex> lock(_repositories_mtx);
    _repositories.emplace(repository->name, repository);
    repository->registered();
}


template<typename T>
std::map<std::string, std::shared_ptr<T>> Broker::_from(const std::initializer_list<std::shared_ptr<T>>& values){
    std::map<std::string,std::shared_ptr<T>> results;
    for (auto& e : values)
        results.emplace(e->name, e);
    return results;
}
