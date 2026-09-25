#ifndef BROKER_BROKER_REPOSITORIES_H
#define BROKER_BROKER_REPOSITORIES_H

#include <memory>
#include <mutex>
#include <map>

#include "../repository/broker_repository.h"

class BrokerRepositories : private virtual BrokerCmd{

public:

    std::shared_ptr<BrokerRepository> operator[](const std::string& name);
    std::shared_ptr<BrokerRepository> find(const std::string& name);

    void emplace(const std::shared_ptr<BrokerRepository>& repository);
    void remove(const std::shared_ptr<BrokerRepository>& repository);

protected:
    explicit BrokerRepositories(const std::map<std::string,std::shared_ptr<BrokerRepository>>& repositories);

    void _publish_registered();

    void _publish_session_open(const std::shared_ptr<BrokerSessionS>& session);
    void _publish_session_close(const std::shared_ptr<BrokerSessionS>& session);

private:
    std::mutex _repositories_mtx;
    std::map<std::string, std::shared_ptr<BrokerRepository>> _repositories;
};
#endif //BROKER_BROKER_REPOSITORIES_H
