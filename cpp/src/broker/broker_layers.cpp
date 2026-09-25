#include "../../include/broker/broker_layers.h"

BrokerLayers::BrokerLayers(const std::map<std::string, std::shared_ptr<Layer>>& layers){
    for (const auto& l : layers){
        _layers[l.first] = LayerElement{
            .layer = l.second,
            .log_cb = l.second->emplace([this](const std::string& name, EventLogType type, const std::string& message){
                on_log(name, type, message);
            }),
            .open_session_cb = l.second->emplace([this](const std::string& name, const std::shared_ptr<BrokerSessionS>& session){
                on_session_open(session);
            }),
            .close_session_cb = l.second->emplace([this](const std::string& name, const std::shared_ptr<BrokerSessionS>& session, const std::string& reason){
                on_session_close(session);
            }),
            .message_cb = l.second->emplace([this](const std::shared_ptr<BrokerSessionS>& session, const ProtoMessage& msg, const LayerReplyFnc& reply_fnc){
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
