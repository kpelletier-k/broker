#include "../../include/broker/broker_tags.h"

BrokerTags::BrokerTags(const std::map<std::string, std::shared_ptr<Tag>>& tags){

    emplace_cmd("tag_register", [this](const std::shared_ptr<BrokerSession>& session, const ProtoMessage& msg, const LayerReplyFnc& reply_fnc){
        return _tag_register(session, msg,reply_fnc);});
    emplace_cmd("tag_unregister", [this](const std::shared_ptr<BrokerSession>& session, const ProtoMessage& msg, const LayerReplyFnc& reply_fnc){
        return _tag_unregister(session, msg,reply_fnc);});
    emplace_cmd("tag_value", [this](const std::shared_ptr<BrokerSession>& session, const ProtoMessage& msg, const LayerReplyFnc& reply_fnc){
        return _tag_value(session,msg,reply_fnc);});

    for(auto& e : tags)
        _emplace(e.first, e.second);
}

std::shared_ptr<Tag> BrokerTags::operator[](const std::string& name){
    return find(name);}


std::shared_ptr<Tag> BrokerTags::find(const std::string& name){
    std::lock_guard<std::mutex> lock(_tags_mtx);
    const auto tag_it = _tags.find(name);
    if (tag_it != _tags.end())
        return tag_it->second.tag;
    return nullptr;
}

std::shared_ptr<Tag> BrokerTags::find(const std::string& name, const tag_t &default_value){
    return _create(name, default_value);}

std::shared_ptr<Tag> BrokerTags::find(const std::string& name, const TagDataType &default_type){
    return _create(name, default_type);}

std::shared_ptr<Tag> BrokerTags::create(const std::string& name, const tag_t &default_value){
    return _create(name, default_value);}
std::shared_ptr<Tag> BrokerTags::create(const std::string& name, const TagDataType &default_type){
    return _create(name, default_type);}

std::shared_ptr<Tag> BrokerTags::_find(const std::shared_ptr<Tag>& params){
    auto tag_name = params->operator[]("name");
    if (!tag_name || !tag_name->is(TagDataType::tag_string))
        return nullptr;

    return find(tag_name->value<tag_string>());
}

void BrokerTags::_update(const std::shared_ptr<Tag>& params, const std::shared_ptr<Tag>& target){
    auto tag_value = params->operator[]("value");
    if (tag_value)
        target->value(tag_value->value_t());
}

bool BrokerTags::_tag_register(const std::shared_ptr<BrokerSession>& session, const ProtoMessage& msg, const LayerReplyFnc& reply_fnc){
    return false;
}

bool BrokerTags::_tag_unregister(const std::shared_ptr<BrokerSession>& session, const ProtoMessage& msg, const LayerReplyFnc& reply_fnc){
    return false;
}

bool BrokerTags::_tag_value(const std::shared_ptr<BrokerSession>& session, const ProtoMessage& msg, const LayerReplyFnc& reply_fnc){
    const auto tag = _find(msg.parameters);
    if (!tag){
        reply_fnc(ProtoError("Tag not found", msg.id));
        return false;
    }
    _update(msg.parameters, tag);
    reply_fnc(ProtoResult(tag, msg.id));
    return true;
}

template<typename T>
std::shared_ptr<Tag> BrokerTags::_create(const std::string& name, const T &default_t){
    std::lock_guard<std::mutex> lock(_tags_mtx);
    const auto tag_it = _tags.find(name);
    if (tag_it != _tags.end())
        return tag_it->second.tag;
    auto new_tag = std::make_shared<Tag>(name, default_t);
    _emplace(name, new_tag);
    return new_tag;
}

void BrokerTags::_emplace(const std::string& name, const std::shared_ptr<Tag>& new_tag){
    _tags[name] = TagElement{
        .tag = new_tag,
        .value_fnc = new_tag->emplace([](const Tag& tag){}),
        .type_fnc = new_tag->emplace([](TagDataType old_type, TagDataType new_type){})
    };
}