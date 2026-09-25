#include "../../include/broker/broker_tags.h"

#include <algorithm>

BrokerTags::BrokerTags(const std::list<std::shared_ptr<Tag>>& tags){

    emplace_cmd("tag_register", [this](const std::shared_ptr<BrokerSessionS>& session, const ProtoMessage& msg, const LayerReplyFnc& reply_fnc){
        return _tag_register(session, msg,reply_fnc);});
    emplace_cmd("tag_unregister", [this](const std::shared_ptr<BrokerSessionS>& session, const ProtoMessage& msg, const LayerReplyFnc& reply_fnc){
        return _tag_unregister(session, msg,reply_fnc);});
    emplace_cmd("tag_value", [this](const std::shared_ptr<BrokerSessionS>& session, const ProtoMessage& msg, const LayerReplyFnc& reply_fnc){
        return _tag_value(session,msg,reply_fnc);});

    for(auto& e : tags)
        _emplace(nullptr, e);
}

// std::shared_ptr<Tag> BrokerTags::operator[](const std::string& name){
//     return find(name);}
//
//
// std::shared_ptr<Tag> BrokerTags::find(const std::string& name){
//     std::lock_guard<std::mutex> lock(_tags_mtx);
//     const auto tag_it = std::find_if(_tags.begin(), _tags.end(), [&](const std::shared_ptr<TagElement>& e){
//         return e->tag->name == name;
//     });
//     if (tag_it != _tags.end())
//         return (*tag_it)->tag;
//     return nullptr;
// }
//
// std::shared_ptr<Tag> BrokerTags::find(const std::string& name, const tag_t &default_value){
//     return _create(name, default_value);}
//
// std::shared_ptr<Tag> BrokerTags::find(const std::string& name, const TagDataType &default_type){
//     return _create(name, default_type);}
//
// std::shared_ptr<Tag> BrokerTags::create(const std::string& name, const tag_t &default_value){
//     return _create(name, default_value);}
// std::shared_ptr<Tag> BrokerTags::create(const std::string& name, const TagDataType &default_type){
//     return _create(name, default_type);}
//


bool BrokerTags::_update(const std::shared_ptr<Tag>& params, const std::shared_ptr<Tag>& target){
    const auto tag_value = params->operator[]("value");
    if (!tag_value)
        return false;

    target->value(tag_value->value_t());
    return true;
}
void BrokerTags::_on_session_open(const std::shared_ptr<BrokerSessionS>& session){}

void BrokerTags::_on_session_close(const std::shared_ptr<BrokerSessionS>& session){
    std::lock_guard<std::mutex> lck(_tags_mtx);
    std::list<std::shared_ptr<TagElement>> remove_tags;
    for (auto& it : _tags){
        it->sessions.remove(session);
        if (it->sessions.empty())
            remove_tags.emplace_back(it);
    }
    // for (const auto e : remove_tags)
    //     BeckhoffAdsIds::remove(e);
}


bool BrokerTags::_tag_register(const std::shared_ptr<BrokerSessionS>& session, const ProtoMessage& msg, const LayerReplyFnc& reply_fnc){
    if (!msg.parameters->is(TagDataType::tag_string)){ // todo, accept tag attribute here
        reply_fnc(ProtoError(ProtoErrorCode::custom_error, "tag_register only accept string params!", msg.id));
        return false;
    }

    //publish(new_tag);

    reply_fnc(ProtoError(ProtoErrorCode::custom_error, "tag: " + msg.parameters->value<std::string>() + " not exist!", msg.id));
    return false;
}

bool BrokerTags::_tag_unregister(const std::shared_ptr<BrokerSessionS>& session, const ProtoMessage& msg, const LayerReplyFnc& reply_fnc){
    if (!msg.parameters->is(TagDataType::tag_string)){ // todo, accept tag attribute here
        reply_fnc(ProtoError(ProtoErrorCode::custom_error, "tag_unregister only accept string params!", msg.id));
        return false;
    }

    reply_fnc(ProtoError(ProtoErrorCode::custom_error, "tag: " + msg.parameters->value<std::string>() + " not registered!", msg.id));
    return false;
}

bool BrokerTags::_tag_value(const std::shared_ptr<BrokerSessionS>& session, const ProtoMessage& msg, const LayerReplyFnc& reply_fnc){
    if (!_is_valid_value(msg.parameters)){ // todo, accept tag attribute here
        reply_fnc(ProtoError(ProtoErrorCode::custom_error, "tag_value only accept struct params {name,value}!", msg.id));
        return false;
    }

    const auto tag = _find(msg.parameters->at("name"));
    if (!tag){
        reply_fnc(ProtoError::custom("Tag not found", msg.id));
        return false;
    }
    if (_update(msg.parameters, tag->tag)){
        reply_fnc(ProtoResult(tag->tag, msg.id));
        return true;
    }

    reply_fnc(ProtoError::custom("Tag: " + tag->tag->name + " update fail!", msg.id));
    return false;
}

void BrokerTags::_emplace(const std::shared_ptr<BrokerSessionS>& session, const std::shared_ptr<Tag>& tag){
    const auto t_e = std::make_shared<TagElement>(session, tag);
    t_e->value_fnc = tag->emplace([session](const std::shared_ptr<Tag>& value){
        session->msg_fnc(ProtoMessage{.name = "tag_value", .parameters = value});
    });
    //todo add type change
    _emplace(t_e);
}

void BrokerTags::_emplace(const std::shared_ptr<TagElement>& tag){
    std::lock_guard<std::mutex> lock(_tags_mtx);
    _tags.emplace_back(tag);
}

std::shared_ptr<BrokerTags::TagElement> BrokerTags::_find(const std::shared_ptr<Tag>& params){
    //     auto tag_name = params->operator[]("name");
    //     if (!tag_name || !tag_name->is(TagDataType::tag_string))
    return nullptr;
    //
    //     return find(tag_name->value<tag_string>());
}

bool BrokerTags::_is_valid_register(const std::shared_ptr<Tag>& params){return false;}
bool BrokerTags::_is_valid_unregister(const std::shared_ptr<Tag>& params){return false;}

bool BrokerTags::_is_valid_value(const std::shared_ptr<Tag>& params){
    return params->is(TagDataType::tag_struct) &&
            params->contain("name") &&
            params->contain("value");
}
