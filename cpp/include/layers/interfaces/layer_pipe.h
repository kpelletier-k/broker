#ifndef BROKER_LAYER_PIPE_H
#define BROKER_LAYER_PIPE_H

#include "../layer.h"

class LayerPipe : public Layer{
public:
    explicit LayerPipe(const std::string& name, const std::shared_ptr<Protocol>& protocol, const std::string& path = "");
};
#endif //BROKER_LAYER_PIPE_H
