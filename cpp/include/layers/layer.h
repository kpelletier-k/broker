#ifndef BROKER_LAYER_H
#define BROKER_LAYER_H

#include <string>

class Layer{
public:
    const std::string name;

protected:
    explicit Layer(const std::string& name) : name(name){}
};

#endif //BROKER_LAYER_H
