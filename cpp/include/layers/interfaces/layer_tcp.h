#ifndef BROKER_LAYER_TCP_H
#define BROKER_LAYER_TCP_H

#include <map>

#include "../layer.h"
#include <socket_tcp_stack.h>

using namespace PeripheralIO;

class LayerTcp : public Layer{
public:
    explicit LayerTcp(const std::string& name, const std::shared_ptr<Protocol>& protocol, const SocketAddressV4& ipv);
    explicit LayerTcp(const std::string& name, const std::shared_ptr<Protocol>& protocol, const SocketInterfaceV4& nic);

    void attached() override;
    void detached() override;
private:
    SocketTcpStack _socket_tcp;

    using EventLog::publish;
    using LayerOpenSession::publish;
    using LayerCloseSession::publish;
    using LayerMessage::publish;

    EventLog::OnEventStorageFnc _log_cb;
    EventConnection::OnEventStorageFnc _conn_cb;
    EventReceived::OnEventStorageFnc _recv_cb;

    std::mutex _sessions_mtx;
    std::map<std::shared_ptr<SocketDescriptor>, std::shared_ptr<BrokerSession>> _sessions;

    std::shared_ptr<BrokerSession> _make_session(const std::shared_ptr<SocketDescriptor>& client);
    std::shared_ptr<BrokerSession> _find_session(const std::shared_ptr<SocketDescriptor>& client);

    void _walk(const std::shared_ptr<BrokerSession>& session, const std::shared_ptr<SocketDescriptor>& client, const ProtoMessage& msg);

    void _on_connected(const std::string& name, const std::shared_ptr<SocketDescriptor>& client);
    void _on_disconnected(const std::string& name, const std::shared_ptr<SocketDescriptor>& client);
};

#endif //BROKER_LAYER_TCP_H
