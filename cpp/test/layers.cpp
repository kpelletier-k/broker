#include <gtest/gtest.h>
#include "../include/broker.h"
#include "../include/layers/interfaces/layer_tcp.h"

class TestLayerTcp : public LayerTcp{
public:

    TestLayerTcp():LayerTcp("layer", SocketAddressV4{"127.0.0.1",14234}){}

    void attached() override{
        LayerTcp::attached();
    }

    void detached() override{
        LayerTcp::detached();
    }
};

TEST(BrokerLayers, layer_init_map){
    // const std::map<std::string,std::shared_ptr<Layer>> layers = {{"layer", std::make_shared<LayerPipe>("layer")}};
    // Broker broker(layers, {});
    //todo
}

TEST(BrokerLayers, layer_init_list){
    const auto layer = std::make_shared<TestLayerTcp>();
    Broker broker({{layer->name,layer}}, {}, {}, {});
    //todo

    std::this_thread::sleep_for(std::chrono::seconds(100000));
}