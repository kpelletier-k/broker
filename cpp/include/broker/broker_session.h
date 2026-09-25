#ifndef BROKER_BROKER_SESSION_H
#define BROKER_BROKER_SESSION_H

#include <string>
#include "../layers/protocols/protocol.h"

typedef std::function<void(const ProtoMessage& msg)> SessionNotifyFnc;

struct BrokerSessionS{
    std::string parent;
    SessionNotifyFnc msg_fnc;
};

class BrokerSession{

public:
    virtual ~BrokerSession() = default;
    void session_open(const std::shared_ptr<BrokerSessionS>& session){
        _on_session_open(session);}
    void session_close(const std::shared_ptr<BrokerSessionS>& session){
        _on_session_close(session);}

protected:
    virtual void _on_session_open(const std::shared_ptr<BrokerSessionS>& session) = 0;
    virtual void _on_session_close(const std::shared_ptr<BrokerSessionS>& session) = 0;
};

#endif //BROKER_BROKER_SESSION_H
