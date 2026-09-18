#ifndef BROKER_BROKER_REPOSITORY_H
#define BROKER_BROKER_REPOSITORY_H
#include <string>

#include "../broker/broker_cmd.h"
#include "../broker/broker_tags.h"
#include "../broker/broker_session.h"

class BrokerRepository : protected BrokerTags, public virtual BrokerCmd{

public:
    const std::string name;

    void registered();
    void unregistered();

    void session_open(const std::shared_ptr<BrokerSession>& session);
    void session_close(const std::shared_ptr<BrokerSession>& session);
protected:
    explicit BrokerRepository(const std::string& name,
                    const std::map<std::string, CmdCallFnc>& callers,
                    const std::map<std::string,std::shared_ptr<Tag>>& tags):
    BrokerTags(tags), name(name){
        for (const auto& e : callers)
            emplace_cmd(e.first, e.second);
    }

    virtual void _registered() = 0;
    virtual void _unregistered() = 0;
    virtual void _session_open(const std::shared_ptr<BrokerSession>& session) = 0;
    virtual void _session_close(const std::shared_ptr<BrokerSession>& session) = 0;


};

#endif //BROKER_BROKER_REPOSITORY_H
