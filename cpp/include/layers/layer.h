#ifndef BROKER_LAYER_H
#define BROKER_LAYER_H

#include <string>
#include <log_events.h>
#include "session/layer_session.h"

typedef Event<void, const std::string&, const std::shared_ptr<LayerSession>&> LayerNewSession;

class Layer : public EventLog, public LayerNewSession{
public:
    const std::string name;

    using EventLog::emplace;
    using LayerNewSession::emplace;

    virtual void attached() = 0;
    virtual void detached() = 0;
protected:
    explicit Layer(const std::string& name) : name(name){}
};

#endif //BROKER_LAYER_H
