#ifndef BROKER_BROKER_LAYERS_H
#define BROKER_BROKER_LAYERS_H

#include <memory>
#include <map>
#include "../layers/layer.h"

class BrokerLayers{

private:

    struct LayerElement{
        std::shared_ptr<Layer> layer;
        EventLog::OnEventStorageFnc log_cb;
        LayerOpenSession::OnEventStorageFnc open_session_cb;
        LayerCloseSession::OnEventStorageFnc close_session_cb;
        LayerMessage::OnEventStorageFnc message_cb;
    };
    std::map<std::string, LayerElement> _layers;

protected:
    explicit BrokerLayers(const std::map<std::string, std::shared_ptr<Layer>>& layers);
    ~BrokerLayers();

    void _publish_attached();
    void _publish_detached();

    virtual void on_session_open(const std::shared_ptr<BrokerSession>& session) = 0;
    virtual void on_session_close(const std::shared_ptr<BrokerSession>& session) = 0;

    virtual void on_log(const std::string& name, EventLogType type, const std::string& message) = 0;
    virtual bool on_message(const std::shared_ptr<BrokerSession>& session, const ProtoMessage& method, const LayerReplyFnc& reply_fnc) = 0; // return success
};
#endif //BROKER_BROKER_LAYERS_H
