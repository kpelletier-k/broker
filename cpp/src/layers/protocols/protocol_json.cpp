#include "../../../include/layers/protocols/protocol_json.h"

std::optional<ProtoMessage> ProtocolJson::parse(const std::vector<uint8_t>& data){
    const auto json = Json::parse(std::string(data.begin(), data.end()));
    if (!_is_valid(json))
        return {};

    if (json.contain("method"))
        return _parse_method(json);
    return {};
}

std::vector<uint8_t> ProtocolJson::parse(const ProtoMessage& proto){
    const auto str = _dump(proto);
    return {str.begin(), str.end()};
}

std::vector<uint8_t> ProtocolJson::parse(const ReplyProto& proto){
    const auto str = _dump(proto);
    return {str.begin(), str.end()};
}

bool ProtocolJson::_is_valid(const Json& json){
    return json.is_object();
}

ProtoMessage ProtocolJson::_parse_method(const Json& json){
    ProtoMessage msg = {
        .name = json["method"].to_string(),
        .parameters = _dump("params",json["params"])
    };
    if (json.contain("method"))
        msg.id = _dump("id",json["id"]);
    return msg;
}


std::shared_ptr<Tag> ProtocolJson::_dump(const std::string& name, const Json& json){
    const auto value = json.value();
    switch (value.index()){
    case 1://json_bool
        return std::make_shared<Tag>(name, get<json_bool>(value));
    case 2://json_long:
        return std::make_shared<Tag>(name, get<json_long>(value));
    case 3://json_double:
        return std::make_shared<Tag>(name, get<json_double>(value));
    case 4://json_string:
        return std::make_shared<Tag>(name, get<json_string>(value));
    case 5://json_array:
        return _dump(name, get<json_array>(value));
    case 6://json_object:
        return _dump(name, get<json_object>(value));

    default:
        return nullptr;
    }
}

std::shared_ptr<Tag> ProtocolJson::_dump(const std::string& name, const json_array& json){
    return nullptr;//todo
}

std::shared_ptr<Tag> ProtocolJson::_dump(const std::string& name, const json_object& json){
    tag_struct results;
    for (const auto& value : json)
        results.emplace_back(_dump(value.first, value.second));
    return std::make_shared<Tag>(name, results);
}

std::string ProtocolJson::_dump(const ProtoMessage& msg){
    Json json{
        {"method", msg.name},
        {"params", _dump(msg.parameters)}
    };
    if (msg.id)
        json["id"] = _dump(msg.id);
    return json.dump();
}

std::string ProtocolJson::_dump(const ReplyProto& msg){
    switch (msg.index()){
    case 0: return _dump(std::get<ProtoResult>(msg)).dump();
    case 1: return _dump(std::get<ProtoError>(msg)).dump();
    default:
        return "";// todo
    }
}

json_any ProtocolJson::_dump(const std::shared_ptr<Tag>& tag){
    if (!tag)return {};

    switch (tag->type()){
        case TagDataType::tag_bool:     return {tag->value<bool>()};
        case TagDataType::tag_int8:     return {tag->value<tag_int8>()};
        case TagDataType::tag_int16:    return {tag->value<tag_int16>()};
        case TagDataType::tag_int32:    return {tag->value<tag_int32>()};
        case TagDataType::tag_int64:    return {tag->value<tag_int64>()};
        case TagDataType::tag_uint8:    return {tag->value<tag_uint8>()};
        case TagDataType::tag_uint16:   return {tag->value<tag_uint16>()};
        case TagDataType::tag_uint32:   return {tag->value<tag_uint32>()};
        case TagDataType::tag_uint64:   return {static_cast<int64_t>(tag->value<tag_uint64>())};
        case TagDataType::tag_float32:  return {tag->value<float>()};
        case TagDataType::tag_float64:  return {tag->value<double>()};
        case TagDataType::tag_string:   return {tag->value<json_string>()};
        //case TagDataType::tag_wstring:
        //case TagDataType::tag_buffer:
        //case TagDataType::tag_array:
        //case TagDataType::tag_struct:

        default:
            return {};
    }
}


Json ProtocolJson::_dump(const ProtoResult& msg){
    return Json{
        {"result", _dump(msg.value)},
        {"id", _dump(msg.id)}
    };
}

Json ProtocolJson::_dump(const ProtoError& msg){
    return Json{
        {"error", msg.msg},
        {"id", _dump(msg.id)}
    };
}

