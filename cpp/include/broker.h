#ifndef BROKER_BROKER_H
#define BROKER_BROKER_H

#include "broker/broker_tags.h"
#include "broker/broker_cmd.h"
#include "broker/broker_layers.h"
#include "broker/broker_repositories.h"
#include "broker/broker_session.h"

class Broker: public EventLog, BrokerLayers, BrokerRepositories, BrokerTags, public virtual BrokerCmd{

public:

    using EventLog::emplace;
    using BrokerRepositories::emplace;

    const std::string name;

    explicit Broker(const std::string& name,
                    const std::list<std::shared_ptr<Layer>>& layers = {},
                    const std::list<std::shared_ptr<BrokerRepository>>& repositories = {},
                    const std::map<std::string, CmdCallFnc>& callers = {},
                    const std::list<std::shared_ptr<Tag>>& tags = {});

    ~Broker() override;

    std::shared_ptr<BrokerRepository> repository(const std::string& name);

protected:
    void on_session_open(const std::shared_ptr<BrokerSessionS>& session) override;
    void on_session_close(const std::shared_ptr<BrokerSessionS>& session) override;
    void on_log(const std::string& name, EventLogType type, const std::string& message) override;
    bool on_message(const std::shared_ptr<BrokerSessionS>& session, const ProtoMessage& method, const LayerReplyFnc& reply_fnc) override;

private:

    std::list<std::shared_ptr<BrokerSessionS>> _sessions;

    bool _is_local_broker(const std::vector<std::string>& paths) const;
    bool _call_local_broker(const std::vector<std::string>& paths, const std::shared_ptr<BrokerSessionS>& session, const ProtoMessage& method, const LayerReplyFnc& reply_fnc);
    bool _call_local_repo(const std::vector<std::string>& paths, const std::shared_ptr<BrokerSessionS>& session, const ProtoMessage& method, const LayerReplyFnc& reply_fnc);

    static std::vector<std::string> _split(const std::string& str, char delimiter = '.');

    template<typename T>
    static std::map<std::string, std::shared_ptr<T>> _to(const std::list<std::shared_ptr<T>>& elements);
};

#endif //BROKER_BROKER_H
