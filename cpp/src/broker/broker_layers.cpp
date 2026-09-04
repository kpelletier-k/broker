#include "../../include/broker/broker_layers.h"

BrokerLayers::BrokerLayers(const std::map<std::string, std::shared_ptr<Layer>>& layers){
    for (const auto& l : layers){
        _layers[l.first] = LayerElement{
            .layer = l.second,
            .log_cb = l.second->emplace([this](const std::string& name, EventLogType type, const std::string& message){
                on_log(name, type, message);
            }),
            .session_cb = l.second->emplace([this](const std::string& name, const std::shared_ptr<LayerSession>& session){
                int y = 0;//on_log(name, type, message);
            })
        };
    }
}

void BrokerLayers::_publish_attached(){
    for (const auto& l : _layers)
        l.second.layer->attached();
}

void BrokerLayers::_publish_detached(){
    for (const auto& l : _layers)
        l.second.layer->detached();
}
