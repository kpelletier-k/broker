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
        LayerNewSession::OnEventStorageFnc session_cb;
    };
    std::map<std::string, LayerElement> _layers;

protected:
    explicit BrokerLayers(const std::map<std::string, std::shared_ptr<Layer>>& layers);

    void _publish_attached();
    void _publish_detached();

    virtual void on_log(const std::string& name, EventLogType type, const std::string& message) = 0;
};
#endif //BROKER_BROKER_LAYERS_H
