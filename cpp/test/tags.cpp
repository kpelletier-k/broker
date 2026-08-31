#include <gtest/gtest.h>
#include "../include/broker_repository_tag.h"


class BrokerRepositoryTagTest : public BrokerRepositoryTag{

};

TEST(BrokerTag, create){

    BrokerRepositoryTagTest broker;

    auto tag_0 = broker.create("tag_0", 1234);
    auto tag_1 = broker.create("tag_1", TagDataType::tag_float32);

    ASSERT_TRUE(tag_0 != nullptr);
    ASSERT_TRUE(tag_1 != nullptr);
}
