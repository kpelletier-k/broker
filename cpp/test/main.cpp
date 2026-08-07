#include "../include/broker.h"
#include "../include/broker_repository.h"
#include "../include/broker_caller.h"
#include <gtest/gtest.h>


class TestRepo : public BrokerRepository{

public:
    TestRepo():BrokerRepository("test_repo"){}
};

TEST(Broker, repository_init_map){

    const auto repo = std::make_shared<TestRepo>();

    bool reg_pass = false;
    auto reg_cb = repo->register_registered([&](){reg_pass = true;});

    bool un_reg_pass = false;
    auto un_reg_cb = repo->register_unregistered([&](){un_reg_pass = true;});

    std::map<std::string,std::shared_ptr<BrokerRepository>> repositories = {{repo->name, repo}};
    Broker broker({},{repo});

    ASSERT_TRUE(reg_pass);
    ASSERT_FALSE(un_reg_pass);
}

TEST(Broker, repository_init_list){

    const auto repo = std::make_shared<TestRepo>();

    bool reg_pass = false;
    auto reg_cb = repo->register_registered([&](){reg_pass = true;});

    bool un_reg_pass = false;
    auto un_reg_cb = repo->register_unregistered([&](){un_reg_pass = true;});

    Broker broker({},{repo});

    ASSERT_TRUE(reg_pass);
    ASSERT_FALSE(un_reg_pass);
}

TEST(Broker, repository_emplace){

    const auto repo = std::make_shared<TestRepo>();

    bool reg_pass = false;
    auto reg_cb = repo->register_registered([&](){reg_pass = true;});

    bool un_reg_pass = false;
    auto un_reg_cb = repo->register_unregistered([&](){un_reg_pass = true;});

    Broker broker({}, {});
    broker.emplace(repo);
    ASSERT_TRUE(reg_pass);
    ASSERT_FALSE(un_reg_pass);
}