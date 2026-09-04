#include "../../../include/layers/interfaces/layer_tcp.h"

LayerTcp::LayerTcp(const std::string& name, const SocketAddressV4& ipv) :
    Layer(name), _socket_tcp(SocketTcpStack(name, ipv)){}
LayerTcp::LayerTcp(const std::string& name, const SocketInterfaceV4& nic) :
    Layer(name), _socket_tcp(SocketTcpStack(name, nic)){}

void LayerTcp::attached(){
    if (_recv_cb)return; // already attached

    _log_cb = _socket_tcp.emplace([this](const std::string& name, EventLogType type, const std::string& msg){
        publish(name, type, msg);
    });

    _conn_cb = _socket_tcp.emplace([this](const std::string& name, EventConnectionState state, const std::shared_ptr<SocketDescriptor>& desc){
        std::string msg = desc->ipv->address() + ":" + std::to_string(desc->ipv->port());
        if (state == EventConnectionState::connected) msg += " connected";
        else  msg += " disconnected";

        publish(this->name, EventLogType::info, msg);
    });

    _recv_cb = _socket_tcp.emplace([](const std::string& name, const std::shared_ptr<SocketDescriptor>& client, const std::shared_ptr<std::vector<uint8_t>>& data){
        int y = 0;
    });

    _socket_tcp.bind();
}
void LayerTcp::detached(){}