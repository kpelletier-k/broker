#ifndef BROKER_LAYER_TCP_H
#define BROKER_LAYER_TCP_H

#include "layer.h"

class LayerTcp : public Layer{
public:
    explicit LayerTcp(const std::string& name) : Layer(name){}
};

#endif //BROKER_LAYER_TCP_H
