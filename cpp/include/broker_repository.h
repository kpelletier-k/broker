#ifndef BROKER_BROKER_REPOSITORY_H
#define BROKER_BROKER_REPOSITORY_H
#include <string>
#include <event_repository.h>

class BrokerRepository : public EventRepository{

    DeclareVoidRepoFnc(Registered, registered,(),())
    DeclareVoidRepoFnc(Unregistered, unregistered,(),())

public:
    const std::string name;

    void registered();
    void unregistered();
protected:
    explicit BrokerRepository(const std::string& name):name(name){}

    virtual void on_registered() = 0;
    virtual void on_unregistered() = 0;
};

#endif //BROKER_BROKER_REPOSITORY_H
