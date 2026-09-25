#include "../include/broker.h"
#include <sstream>

Broker::Broker(const std::string& name,
                const std::list<std::shared_ptr<Layer>>& layers,
                const std::list<std::shared_ptr<BrokerRepository>>& repositories,
                const std::map<std::string, CmdCallFnc>& callers,
                const std::list<std::shared_ptr<Tag>>& tags) :
        BrokerLayers(_to(layers)),
        BrokerRepositories(_to(repositories)),
        BrokerTags(tags),
        name(name){
    for (const auto& e : callers)
        emplace_cmd(e.first, e.second);
    _publish_attached();
    _publish_registered();
}

Broker::~Broker(){
    _publish_detached();
}


std::shared_ptr<BrokerRepository> Broker::repository(const std::string& name){
    return BrokerRepositories::operator[](name);}

void Broker::on_session_open(const std::shared_ptr<BrokerSessionS>& session){
    _publish_session_open(session);}

void Broker::on_session_close(const std::shared_ptr<BrokerSessionS>& session){
    _publish_session_close(session);}

void Broker::on_log(const std::string& name, EventLogType type, const std::string& message){
    EventLog::publish(name, type, message);}

bool Broker::_is_local_broker(const std::vector<std::string>& paths) const{
    if (paths.empty()) return false; //todo, double checked
    return paths.size() == 1 || paths.front() == name;
}

bool Broker::_call_local_broker(const std::vector<std::string>& paths, const std::shared_ptr<BrokerSessionS>& session, const ProtoMessage& method, const LayerReplyFnc& reply_fnc){
    auto fnc = find_cmd(paths.back());
    if (fnc)
        return fnc(session, method, reply_fnc);
    reply_fnc(ProtoError{.msg = "repository function " + paths.back() + " not found", .id = method.id});
    return false;
}

bool Broker::_call_local_repo(const std::vector<std::string>& paths, const std::shared_ptr<BrokerSessionS>& session, const ProtoMessage& method, const LayerReplyFnc& reply_fnc){
    const auto repo = BrokerRepositories::find(paths.front());
    if (repo){
        auto fnc = repo->find_cmd(paths.back());
        if (fnc)
            return fnc(session, method, reply_fnc);
        reply_fnc(ProtoError{.msg = "repository function " + paths.back() + " not found", .id = method.id});
    }else
        reply_fnc(ProtoError{.msg = "repository " + paths.front() + " not found", .id = method.id});
    return false;
}

bool Broker::on_message(const std::shared_ptr<BrokerSessionS>& session, const ProtoMessage& method, const LayerReplyFnc& reply_fnc){
    const auto paths = _split(method.name);
    if (paths.empty()){
        reply_fnc(ProtoError{.msg = "fail to parse command name", .id = method.id});
        return false;
    }

    if (_is_local_broker(paths))
        return _call_local_broker(paths, session, method, reply_fnc);
    return _call_local_repo(paths, session, method, reply_fnc);
}


std::vector<std::string> Broker::_split(const std::string& str, char delimiter){
    std::stringstream ss(str);
    std::string token;
    std::vector<std::string> results;

    while (std::getline(ss, token, delimiter)) {
        results.push_back(token);
    }
    return results;
}

template<typename T>
std::map<std::string, std::shared_ptr<T>> Broker::_to(const std::list<std::shared_ptr<T>>& elements){
    std::map<std::string, std::shared_ptr<T>> results;
    for (const auto& element : elements)
        results[element->name] = element;
    return results;
}
