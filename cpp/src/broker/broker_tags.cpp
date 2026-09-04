#include "../../include/broker/broker_tags.h"

BrokerTags::BrokerTags(const std::map<std::string, std::shared_ptr<Tag>>& tags){
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

template<typename T>
std::shared_ptr<Tag> BrokerTags::_create(const std::string& name, const T &default_t){
    std::lock_guard<std::mutex> lock(_tags_mtx);
    const auto tag_it = _tags.find(name);
    if (tag_it != _tags.end())
        return tag_it->second.tag;
    auto new_tag = std::make_shared<Tag>(default_t);
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