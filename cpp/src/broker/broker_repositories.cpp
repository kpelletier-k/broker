#include "../../include/broker/broker_repositories.h"


BrokerRepositories::BrokerRepositories(const std::map<std::string,std::shared_ptr<BrokerRepository>>& repositories):
    _repositories(repositories){}

std::shared_ptr<BrokerRepository> BrokerRepositories::operator[](const std::string& name){
    std::lock_guard<std::mutex> lock(_repositories_mtx);
    const auto it = _repositories.find(name);
    return it != _repositories.end() ? it->second : nullptr;
}

void BrokerRepositories::emplace(const std::shared_ptr<BrokerRepository>& repository){
    if (!repository || repository->name.empty())return;
    std::lock_guard<std::mutex> lock(_repositories_mtx);
    _repositories.emplace(repository->name, repository);
    repository->registered();
}

void BrokerRepositories::remove(const std::shared_ptr<BrokerRepository>& repository){
    if (!repository || repository->name.empty())return;
    std::lock_guard<std::mutex> lock(_repositories_mtx);
    _repositories.erase(repository->name);
    repository->unregistered();
}

void BrokerRepositories::_publish_registered(){
    for (const auto& repo : _repositories)
        if (repo.second && !repo.first.empty())
            repo.second->registered();
}