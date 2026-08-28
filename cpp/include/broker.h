#ifndef BROKER_BROKER_H
#define BROKER_BROKER_H

#include <map>
#include "broker_repository.h"
#include "layers/layer.h"

class Broker{

public:
    explicit Broker(const std::initializer_list<std::shared_ptr<Layer>>& layers, const std::initializer_list<std::shared_ptr<BrokerRepository>>& repositories);
    explicit Broker(const std::map<std::string,std::shared_ptr<Layer>>& layers, const std::map<std::string,std::shared_ptr<BrokerRepository>>& repositories);

    ~Broker();

    void emplace(const std::shared_ptr<BrokerRepository>& repository);
    void remove(const std::shared_ptr<BrokerRepository>& repository);
private:
    const std::map<std::string, std::shared_ptr<Layer>> _layers;

    std::mutex _repositories_mtx;
    std::map<std::string, std::shared_ptr<BrokerRepository>> _repositories;


    static void _publish_attached(const std::map<std::string, std::shared_ptr<Layer>>& layers);
    static void _publish_detached(const std::map<std::string, std::shared_ptr<Layer>>& layers);
    static void _publish_registered(const std::map<std::string, std::shared_ptr<BrokerRepository>>& repositories);

    template<typename T>
    static std::map<std::string, std::shared_ptr<T>> _from(const std::initializer_list<std::shared_ptr<T>>& values);
};

#endif //BROKER_BROKER_H
