#ifndef BROKER_LAYER_PIPE_H
#define BROKER_LAYER_PIPE_H

#include "layer.h"

class LayerPipe : public Layer{
public:
    explicit LayerPipe(const std::string& name) : Layer(name){}
};
#endif //BROKER_LAYER_PIPE_H
