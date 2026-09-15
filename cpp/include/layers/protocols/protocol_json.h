#ifndef BROKER_PROTOCOL_JSON_H
#define BROKER_PROTOCOL_JSON_H

#include "protocol.h"
#include <json_rpc.h>

class ProtocolJson : public Protocol{

public:
    std::optional<ProtoMessage> parse(const std::vector<uint8_t>& data) override;
    std::vector<uint8_t> parse(const ProtoMessage& proto) override;
    std::vector<uint8_t> parse(const ReplyProto& proto) override;

private:
    static bool _is_valid(const Json& json);

    static ProtoMessage _parse_method(const Json& json);

    static std::shared_ptr<Tag> _dump(const std::string& name, const Json& json);
    static std::shared_ptr<Tag> _dump(const std::string& name, const json_array& json);
    static std::shared_ptr<Tag> _dump(const std::string& name, const json_object& json);

    static std::string _dump(const ProtoMessage& msg);
    static std::string _dump(const ReplyProto& msg);

    static json_any _dump(const std::shared_ptr<Tag>& tag);

    static Json _dump(const ProtoResult& msg);
    static Json _dump(const ProtoError& msg);
};

#endif //BROKER_PROTOCOL_JSON_H
