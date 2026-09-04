#include <gtest/gtest.h>
#include "../include/broker/broker_tags.h"


class BrokerTagsTest : public BrokerTags{

};

TEST(BrokerTags, create){

    BrokerTagsTest broker;

    auto tag_0 = broker.create("tag_0", 1234);
    auto tag_1 = broker.create("tag_1", TagDataType::tag_float32);

    ASSERT_TRUE(tag_0 != nullptr);
    ASSERT_TRUE(tag_1 != nullptr);

    ASSERT_EQ(tag_0, broker["tag_0"]);
    ASSERT_EQ(tag_1, broker["tag_1"]);
}

TEST(BrokerTags, registration){

    BrokerTagsTest broker;

    int register_cnt = 0;
    broker.emplace([&](const std::string& name, const std::shared_ptr<Tag>& tag){
        register_cnt++;
    });

    auto tag_0 = broker.create("tag_0", 1234);

    ASSERT_EQ(register_cnt, 0);


}
