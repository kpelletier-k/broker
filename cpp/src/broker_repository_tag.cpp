#include "../include/broker_repository_tag.h"

BrokerRepositoryTag::BrokerRepositoryTag(const std::map<std::string, std::shared_ptr<Tag>>& tags) :
    _tags{tags} {}


std::shared_ptr<Tag> BrokerRepositoryTag::create(const std::string& name, const tag_t &default_value){
    return _create(name, default_value);}
std::shared_ptr<Tag> BrokerRepositoryTag::create(const std::string& name, const TagDataType &default_type){
    return _create(name, default_type);}

template<typename T>
std::shared_ptr<Tag> BrokerRepositoryTag::_create(const std::string& name, const T &default_t){
    std::lock_guard<std::mutex> lock(_tags_mtx);
    const auto tag_it = _tags.find(name);
    if (tag_it != _tags.end())
        return tag_it->second;
    auto new_tag = std::make_shared<Tag>(default_t);
    _tags[name] = new_tag;
    return new_tag;
}