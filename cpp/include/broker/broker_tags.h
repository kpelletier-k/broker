#ifndef BROKER_BROKER_TAGS_H
#define BROKER_BROKER_TAGS_H

#include <map>
#include <tag.h>
#include "broker_cmd.h"

typedef Event<void, const std::string&, const std::shared_ptr<Tag>&> BrokerTagRegistered; // void (const std::string& name, const std::shared_ptr<Tag>& tag)

class BrokerTags : private virtual BrokerCmd, protected BrokerTagRegistered{

public:

    using BrokerTagRegistered::emplace;

    std::shared_ptr<Tag> find(const std::string& name);
    std::shared_ptr<Tag> find(const std::string& name, const tag_t &default_value);
    std::shared_ptr<Tag> find(const std::string& name, const TagDataType &default_type);

    std::shared_ptr<Tag> create(const std::string& name, const tag_t &default_value);
    std::shared_ptr<Tag> create(const std::string& name, const TagDataType &default_type);

    std::shared_ptr<Tag> operator[](const std::string& name);

protected:
    explicit BrokerTags(const std::map<std::string,std::shared_ptr<Tag>>& tags = {});

private:

    struct TagElement{
        std::shared_ptr<Tag> tag;
        EventChange::OnEventStorageFnc value_fnc;
        EventTypeChange::OnEventStorageFnc type_fnc;
    };

    std::mutex _tags_mtx;
    std::map<std::string, TagElement> _tags;

    std::shared_ptr<Tag> _find(const std::shared_ptr<Tag>& params);
    static void _update(const std::shared_ptr<Tag>& params, const std::shared_ptr<Tag>& target);

    bool _tag_register(const std::shared_ptr<BrokerSession>& session, const ProtoMessage& msg, const LayerReplyFnc& reply_fnc);
    bool _tag_unregister(const std::shared_ptr<BrokerSession>& session, const ProtoMessage& msg, const LayerReplyFnc& reply_fnc);
    bool _tag_value(const std::shared_ptr<BrokerSession>& session, const ProtoMessage& msg, const LayerReplyFnc& reply_fnc);


    template<typename T>
    std::shared_ptr<Tag> _create(const std::string& name, const T &default_t);
    void _emplace(const std::string& name, const std::shared_ptr<Tag>& new_tag);
};

#endif //BROKER_BROKER_TAGS_H
