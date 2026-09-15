#ifndef BROKER_LAYER_WS_H
#define BROKER_LAYER_WS_H

#include "../layer.h"

class LayerWS : public Layer{
public:
    explicit LayerWS(const std::string& name, const std::shared_ptr<Protocol>& protocol) : Layer(name, protocol){}
};

#endif //BROKER_LAYER_WS_H
