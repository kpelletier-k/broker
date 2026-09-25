#ifndef BROKER_BROKER_REPOSITORY_H
#define BROKER_BROKER_REPOSITORY_H
#include <string>

#include "../broker/broker_cmd.h"
#include "../broker/broker_tags.h"
#include "../broker/broker_session.h"

class BrokerRepository : public virtual BrokerTags, public virtual BrokerCmd, public virtual BrokerSession{

public:
    const std::string name;

    void registered();
    void unregistered();

protected:
    explicit BrokerRepository(const std::string& name,
                    const std::map<std::string, CmdCallFnc>& callers,
                    const std::list<std::shared_ptr<Tag>>& tags):
        BrokerTags(tags), BrokerCmd(callers), name(name){}

    virtual void _registered() = 0;
    virtual void _unregistered() = 0;


};

#endif //BROKER_BROKER_REPOSITORY_H
