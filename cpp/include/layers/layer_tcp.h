#ifndef BROKER_LAYER_TCP_H
#define BROKER_LAYER_TCP_H

#include "layer.h"
#include <socket_tcp_stack.h>

using namespace PeripheralIO;

class LayerTcp : public Layer{
public:
    explicit LayerTcp(const std::string& name, const SocketAddressV4& ipv);
    explicit LayerTcp(const std::string& name, const SocketInterfaceV4& nic);

    void attached() override;
    void detached() override;
private:
    SocketTcpStack _socket_tcp;

    EventLog::OnEventStorageFnc _log_cb;
    EventConnection::OnEventStorageFnc _conn_cb;
    EventReceived::OnEventStorageFnc _recv_cb;
};

#endif //BROKER_LAYER_TCP_H
