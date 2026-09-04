#ifndef BROKER_BROKER_H
#define BROKER_BROKER_H

#include "broker/broker_tags.h"
#include "broker/broker_cmd.h"
#include "broker/broker_layers.h"
#include "broker/broker_repositories.h"
#include "broker/broker_session.h"

class Broker: public EventLog, BrokerLayers, BrokerRepositories, BrokerCmd, BrokerTags{

public:
    explicit Broker(const std::map<std::string,std::shared_ptr<Layer>>& layers,
                    const std::map<std::string,std::shared_ptr<BrokerRepository>>& repositories,
                    const std::map<std::string, BrokerCmd::CallerFnc>& callers,
                    const std::map<std::string,std::shared_ptr<Tag>>& tags);

    ~Broker() override;

    std::shared_ptr<Tag> tag(const std::string& name);
    BrokerCmd::CallerFnc cmd(const std::string& name);
    std::shared_ptr<BrokerRepository> repository(const std::string& name);

protected:
    void on_log(const std::string& name, EventLogType type, const std::string& message) override;

private:

    std::list<std::shared_ptr<BrokerSession>> _sessions;
};

#endif //BROKER_BROKER_H
