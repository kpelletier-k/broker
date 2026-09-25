#ifndef BROKER_BROKER_TAGS_H
#define BROKER_BROKER_TAGS_H

#include <map>
#include <tag.h>
#include "broker_cmd.h"

typedef Event<void, const std::shared_ptr<Tag>&> BrokerTagRegistered; // void (const std::string& name, const std::shared_ptr<Tag>& tag)

class BrokerTags : virtual BrokerCmd, virtual protected BrokerSession, protected BrokerTagRegistered{

public:

    using BrokerTagRegistered::emplace;

    struct TagElement{
        std::shared_ptr<Tag> tag;
        EventChange::OnEventStorageFnc value_fnc;
        EventTypeChange::OnEventStorageFnc type_fnc;
        std::list<std::shared_ptr<BrokerSessionS>> sessions;
        TagElement(const std::shared_ptr<BrokerSessionS>& session, const std::shared_ptr<Tag>& tag):
            tag(tag), sessions({session}){}
    };

    // std::shared_ptr<Tag> find(const std::string& name);
    // std::shared_ptr<Tag> find(const std::string& name, const tag_t &default_value);
    // std::shared_ptr<Tag> find(const std::string& name, const TagDataType &default_type);
    //
    // std::shared_ptr<Tag> create(const std::string& name, const tag_t &default_value);
    // std::shared_ptr<Tag> create(const std::string& name, const TagDataType &default_type);
    //
    // std::shared_ptr<Tag> operator[](const std::string& name);

protected:
    explicit BrokerTags(const std::list<std::shared_ptr<Tag>>& tags = {});

    void _on_session_open(const std::shared_ptr<BrokerSessionS>& session) override;
    void _on_session_close(const std::shared_ptr<BrokerSessionS>& session) override;

    bool _tag_register(const std::shared_ptr<BrokerSessionS>& session, const ProtoMessage& msg, const LayerReplyFnc& reply_fnc);
    bool _tag_unregister(const std::shared_ptr<BrokerSessionS>& session, const ProtoMessage& msg, const LayerReplyFnc& reply_fnc);
    bool _tag_value(const std::shared_ptr<BrokerSessionS>& session, const ProtoMessage& msg, const LayerReplyFnc& reply_fnc);

    void _emplace(const std::shared_ptr<BrokerSessionS>& session, const std::shared_ptr<Tag>& tag);
    void _emplace(const std::shared_ptr<TagElement>& tag);

private:
    std::mutex _tags_mtx;
    std::list<std::shared_ptr<TagElement>> _tags;

    std::shared_ptr<TagElement> _find(const std::shared_ptr<Tag>& params);
    virtual bool _update(const std::shared_ptr<Tag>& params, const std::shared_ptr<Tag>& target); // return true if success

    static bool _is_valid_register(const std::shared_ptr<Tag>& params);
    static bool _is_valid_unregister(const std::shared_ptr<Tag>& params);
    static bool _is_valid_value(const std::shared_ptr<Tag>& params);
};

#endif //BROKER_BROKER_TAGS_H
