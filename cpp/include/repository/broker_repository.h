#ifndef BROKER_BROKER_REPOSITORY_H
#define BROKER_BROKER_REPOSITORY_H
#include <string>

#include "../broker/broker_cmd.h"
#include "../broker/broker_tags.h"

class BrokerRepository : protected BrokerTags, public virtual BrokerCmd{

public:
    const std::string name;

    void registered();
    void unregistered();
protected:
    explicit BrokerRepository(const std::string& name,
                    const std::map<std::string, BrokerCmd::CmdCallFnc>& callers,
                    const std::map<std::string,std::shared_ptr<Tag>>& tags):
    BrokerTags(tags), name(name){
        for (const auto& e : callers)
            emplace_cmd(e.first, e.second);
    }

};

#endif //BROKER_BROKER_REPOSITORY_H
