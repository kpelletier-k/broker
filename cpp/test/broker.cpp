#include <gtest/gtest.h>
#include "../include/broker.h"
#include "../include/layers/interfaces/layer_tcp.h"

TEST(Broker, tags){
    auto tag_0 = std::make_shared<Tag>();
    Broker broker({},{},{},{{"tag_0", tag_0}});
    ASSERT_EQ(broker.tag("tag_0"), tag_0);
}

TEST(Broker, commands){

    auto fnc = [&](const BrokerCmdMessage& msg)-> BrokerCmdReply{
        return {};
    };

    Broker broker({},
    {},
    {{"cmd_0", fnc}},
    {});

    auto cmd_0 = broker.cmd("cmd_0");
    ASSERT_TRUE(cmd_0 != nullptr);
    const auto result = cmd_0(BrokerCmdMethod{"123",Tag("poutpout")});
    ASSERT_EQ(result.index(), 0);
}

class Repo : public BrokerRepository{
public:
    Repo(const std::string& name,
        const std::map<std::string, BrokerCmd::CallerFnc>& callers,
        const std::map<std::string,std::shared_ptr<Tag>>& tags):
    BrokerRepository(name, callers, tags) {}
};

TEST(Broker, repositories){
    auto tag_0 = std::make_shared<Tag>();
    auto repo = std::shared_ptr<Repo>(new Repo("repo", {}, {{"tag_0", tag_0}}));

    Broker broker({},{{"repo", repo}},{}, {});
    const auto repo_expected = broker.repository("repo");
    ASSERT_TRUE(repo_expected != nullptr);
}

TEST(Broker, exemple){
    auto repo = std::shared_ptr<Repo>(new Repo("repo", {}, {}));
    auto layer = std::make_shared<LayerTcp>("layer", SocketAddressV4{"127.0.0.1",14234});

    Broker broker({{"layer",layer}},{{"repo", repo}},{}, {});

    std::this_thread::sleep_for(std::chrono::seconds(100));
}