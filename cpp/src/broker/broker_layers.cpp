#include "../../include/broker/broker_layers.h"

BrokerLayers::BrokerLayers(const std::map<std::string, std::shared_ptr<Layer>>& layers){
    for (const auto& l : layers){
        _layers[l.first] = LayerElement{
            .layer = l.second,
            .log_cb = l.second->emplace([this](const std::string& name, EventLogType type, const std::string& message){
                on_log(name, type, message);
            }),
            .open_session_cb = l.second->emplace([this](const std::string& name, const std::shared_ptr<BrokerSession>& session){
                int y = 0;
            }),
            .close_session_cb = l.second->emplace([this](const std::string& name, const std::shared_ptr<BrokerSession>& session, const std::string& reason){
                int y = 0;
            }),
            .message_cb = l.second->emplace([this](const std::shared_ptr<BrokerSession>& session, const ProtoMessage& msg, const LayerReplyFnc& reply_fnc){
                on_message(session, msg, reply_fnc);
            })
        };
    }
}

BrokerLayers::~BrokerLayers(){
    for (const auto& l : _layers)
        l.second.layer->clear();
    _layers.clear();
}

void BrokerLayers::_publish_attached(){
    for (const auto& l : _layers)
        l.second.layer->attached();
}

void BrokerLayers::_publish_detached(){
    for (const auto& l : _layers)
        l.second.layer->detached();
}
