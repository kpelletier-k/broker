#ifndef BROKER_BROKER_REPOSITORY_H
#define BROKER_BROKER_REPOSITORY_H
#include <string>

#include "../broker/broker_cmd.h"
#include "../broker/broker_tags.h"

class BrokerRepository : protected BrokerCmd, protected BrokerTags{

public:
    const std::string name;

    void registered();
    void unregistered();
protected:
    explicit BrokerRepository(const std::string& name,
                    const std::map<std::string, BrokerCmd::CallerFnc>& callers,
                    const std::map<std::string,std::shared_ptr<Tag>>& tags):
        name(name), BrokerCmd(callers), BrokerTags(tags) {}

};

#endif //BROKER_BROKER_REPOSITORY_H
