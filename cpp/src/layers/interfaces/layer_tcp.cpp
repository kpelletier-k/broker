#include "../../../include/layers/interfaces/layer_tcp.h"

LayerTcp::LayerTcp(const std::string& name, const std::shared_ptr<Protocol>& protocol, const SocketAddressV4& ipv) :
    Layer(name, protocol), _socket_tcp(SocketTcpStack(name, ipv)){}
LayerTcp::LayerTcp(const std::string& name, const std::shared_ptr<Protocol>& protocol, const SocketInterfaceV4& nic) :
    Layer(name, protocol), _socket_tcp(SocketTcpStack(name, nic)){}

void LayerTcp::attached(){
    if (_socket_tcp.is_bind())return; // already attached

    _log_cb = _socket_tcp.emplace([this](const std::string& name, EventLogType type, const std::string& msg){
        publish(name, type, msg);
    });

    _conn_cb = _socket_tcp.emplace([this](const std::string& name, EventConnectionState state, const std::shared_ptr<SocketDescriptor>& client){
        if (state == EventConnectionState::connected) _on_connected(name, client);
        else  _on_disconnected(name, client);
    });

    _recv_cb = _socket_tcp.emplace([this](const std::string& name, const std::shared_ptr<SocketDescriptor>& client, const std::shared_ptr<std::vector<uint8_t>>& data){
        if (!data || !protocol)return;// todo, why

        const auto session = _find_session(client);
        if (session){
            const auto msg = protocol->parse(*data);
            if (msg)
                _walk(session, client, msg.value());
            else
                publish(this->name, EventLogType::error, client->ipv->address() + ":" + std::to_string(client->ipv->port()) + " fail to parse protocol data");
        }else
            publish(this->name, EventLogType::warning, client->ipv->address() + ":" + std::to_string(client->ipv->port()) + " was killed session");
    });

    _socket_tcp.bind(); // todo make thread to retry connection
}
void LayerTcp::detached(){
    _recv_cb = nullptr;
    _conn_cb = nullptr;
    _log_cb = nullptr;
    _socket_tcp.descriptor->close();
}

std::shared_ptr<BrokerSessionS> LayerTcp::_make_session(const std::shared_ptr<SocketDescriptor>& client){
    std::lock_guard<std::mutex> lock(_sessions_mtx);
    auto session = std::make_shared<BrokerSessionS>(name, [client, this](const ProtoMessage& msg){
        _socket_tcp.send(client, protocol->parse(msg));
    });
    _sessions[client] = session;
    return session;
}

std::shared_ptr<BrokerSessionS> LayerTcp::_find_session(const std::shared_ptr<SocketDescriptor>& client){
    std::lock_guard<std::mutex> lock(_sessions_mtx);
    const auto it = _sessions.find(client);
    return it != _sessions.end() ? it->second : nullptr;
}

void LayerTcp::_walk(const std::shared_ptr<BrokerSessionS>& session, const std::shared_ptr<SocketDescriptor>& client, const ProtoMessage& msg){

    if (msg.id){
        publish(session, msg, [this, client](const ReplyProto& reply){
            _socket_tcp.send(client, protocol->parse(reply));
        });
    }else
        publish(session, msg, [](const ReplyProto& reply){});
}

void LayerTcp::_on_connected(const std::string& name, const std::shared_ptr<SocketDescriptor>& client){
    const auto session = _make_session(client);
    publish(name, session);
    publish(name, EventLogType::info, client->ipv->address() + ":" + std::to_string(client->ipv->port()) + " connected");
}

void LayerTcp::_on_disconnected(const std::string& name, const std::shared_ptr<SocketDescriptor>& client){
    publish(name, EventLogType::info, client->ipv->address() + ":" + std::to_string(client->ipv->port()) + " disconnected");
    publish(name, _find_session(client), "disconnected");
}