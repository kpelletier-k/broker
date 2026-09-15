#include <gtest/gtest.h>

#include <memory>
#include "../include/broker.h"
#include "../include/layers/interfaces/layer_tcp.h"
#include "../include/layers/protocols/protocol_json.h"

TEST(Broker, tags)
{
    auto tag_0 = std::make_shared<Tag>("tag_0");
    Broker broker("main",{},{},{},{tag_0});
    ASSERT_EQ(broker.tag("tag_0"), tag_0);
}

TEST(Broker, commands){

    // auto fnc = [&](const BrokerCmdMessage& msg)-> BrokerCmdReply{
    //     return {};
    // };
    //
    // Broker broker("main",{},
    // {},
    // {{"cmd_0", fnc}},
    // {});
    //
    // auto cmd_0 = broker.cmd("cmd_0");
    // ASSERT_TRUE(cmd_0 != nullptr);
    // const auto result = cmd_0(BrokerCmdMethod{.id = "123",.args = Tag("poutpout")});
    // ASSERT_EQ(result.index(), 0);
}

class Repo : public BrokerRepository{
public:
    Repo(const std::string& name,
        const std::map<std::string, BrokerCmd::CmdCallFnc>& callers = {},
        const std::map<std::string,std::shared_ptr<Tag>>& tags = {}):
    BrokerRepository(name, callers, tags) {}
};

TEST(Broker, repositories){
    auto tag_0 = std::make_shared<Tag>("tag_0");
    auto repo = std::shared_ptr<Repo>(new Repo("repo", {}, {{tag_0->name,tag_0}}));

    Broker broker("main",{},{repo},{}, {});
    const auto repo_expected = broker.repository("repo");
    ASSERT_TRUE(repo_expected != nullptr);
}

TEST(Broker, exemple){
    auto tag_0 = std::make_shared<Tag>("tag_0");
    tag_0->value(56789);
    auto tag_1 = std::make_shared<Tag>("tag_1");

    auto fnc = [](const std::shared_ptr<BrokerSession>& session, const ProtoMessage& msg, const LayerReplyFnc& reply_fnc){
        return false;
    };

    auto repo = std::shared_ptr<Repo>(new Repo("repo", {{"test_fnc", fnc}}, {{tag_1->name,tag_1}}));
    auto layer = std::make_shared<LayerTcp>("layer", std::make_shared<ProtocolJson>(), SocketAddressV4{"127.0.0.1",14234});

    Broker broker("main",{layer},{repo},{}, {tag_0});

    std::this_thread::sleep_for(std::chrono::seconds(10000));

    //nc 127.0.0.1 14234
    //{"method": "repo.test_fnc", "params":"potatoes", "id":3}
    //{"method": "tag_value", "params":{"name":"tag_0"}, "id":4}
    //{"method": "tag_value", "params":{"name":"tag_0", "value":87654}, "id":5}
    //{"method": "repo.tag_value", "params":{"name":"tag_1", "value":69}, "id":6}
    //{"method": "repo.tag_value", "params":{"name":"tag_1"}, "id":7}

}