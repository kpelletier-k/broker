#ifndef BROKER_LAYER_WS_H
#define BROKER_LAYER_WS_H

#include "layer.h"

class LayerWS : public Layer{
public:
    explicit LayerWS(const std::string& name) : Layer(name){}
};

#endif //BROKER_LAYER_WS_H
