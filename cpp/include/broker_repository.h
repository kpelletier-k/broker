#ifndef BROKER_BROKER_REPOSITORY_H
#define BROKER_BROKER_REPOSITORY_H
#include <string>
#include <state_events.h>

class BrokerRepository : public StateEvent<void>{

public:
    const std::string name;

    void registered();
    void unregistered();
protected:
    explicit BrokerRepository(const std::string& name):name(name){}

};

#endif //BROKER_BROKER_REPOSITORY_H
