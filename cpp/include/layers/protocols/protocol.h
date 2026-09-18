#ifndef BROKER_PROTOCOL_H
#define BROKER_PROTOCOL_H

#include <variant>
#include <vector>
#include <cstdint>
#include <string>
#include <optional>
#include <tag.h>


struct ProtoMessage{
    std::string name;
    std::shared_ptr<Tag> parameters;
    std::shared_ptr<Tag> id;
};

struct ProtoResult{
    std::shared_ptr<Tag> value;
    std::shared_ptr<Tag> id;
};

enum class ProtoErrorCode : uint16_t{
    parse_error = 0x0001,
    cmd_not_exist = 0x0002,
    params_error = 0x0004,
    custom_error = 0x0008
};

struct ProtoError{
    ProtoErrorCode code;
    std::string msg;
    std::shared_ptr<Tag> id;

    static ProtoError custom(const std::string& msg, const std::shared_ptr<Tag>& id){
        return {.code = ProtoErrorCode::custom_error, .msg = msg, .id = id};}
};
typedef std::variant<ProtoResult, ProtoError> ReplyProto;

class Protocol{
public:
    virtual ~Protocol() = default;

    virtual std::optional<ProtoMessage> parse(const std::vector<uint8_t>& data) = 0;
    virtual std::vector<uint8_t> parse(const ProtoMessage& proto) = 0;
    virtual std::vector<uint8_t> parse(const ReplyProto& proto) = 0;
};

#endif //BROKER_PROTOCOL_H
