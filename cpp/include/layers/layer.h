#ifndef BROKER_LAYER_H
#define BROKER_LAYER_H

#include <string>
#include <optional>
#include <log_events.h>

#include "protocols/protocol.h"
#include "../broker/broker_session.h"

typedef Event<void, const std::string&, const std::shared_ptr<BrokerSession>&> LayerOpenSession;//(const std::string& name, const std::shared_ptr<BrokerSession>& session)
typedef Event<void, const std::string&, const std::shared_ptr<BrokerSession>&, const std::string&> LayerCloseSession; //(const std::string& name, const std::shared_ptr<BrokerSession>& session, const std::string& reason)

typedef std::function<void(const ReplyProto& reply)> LayerReplyFnc;
typedef Event<void, const std::shared_ptr<BrokerSession>&, const ProtoMessage&, const LayerReplyFnc&> LayerMessage;//ReplyProto (const std::shared_ptr<BrokerSession>& session, const ProtoMessage& msg, const LayerReplyFnc& reply_fnc)

class Layer : public EventLog, public LayerOpenSession, public LayerCloseSession, public LayerMessage {
public:
    const std::string name;
    const std::shared_ptr<Protocol> protocol;

    using EventLog::emplace;
    using LayerOpenSession::emplace;
    using LayerCloseSession::emplace;
    using LayerMessage::emplace;

    virtual void attached() = 0;
    virtual void detached() = 0;

    inline void clear();

protected:
    explicit Layer(const std::string& name, const std::shared_ptr<Protocol>& protocol) : name(name), protocol{protocol}{}

};

inline void Layer::clear(){
    EventLog::clear();
    LayerOpenSession::clear();
    LayerCloseSession::clear();
    LayerMessage::clear();
}

#endif //BROKER_LAYER_H
