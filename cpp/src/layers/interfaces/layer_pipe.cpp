#include "../../../include/layers/interfaces/layer_pipe.h"

LayerPipe::LayerPipe(const std::string& name, const std::shared_ptr<Protocol>& protocol, const std::string& path) : Layer(name, protocol){}