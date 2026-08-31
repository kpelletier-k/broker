#ifndef BROKER_BROKER_REPOSITORY_TAG_H
#define BROKER_BROKER_REPOSITORY_TAG_H

#include <map>
#include <tag.h>

typedef Event<void> BrokerTagRegistered;

class BrokerRepositoryTag : public BrokerTagRegistered{

public:
    explicit BrokerRepositoryTag(const std::map<std::string,std::shared_ptr<Tag>>& tags = {});

    std::shared_ptr<Tag> create(const std::string& name, const tag_t &default_value);
    std::shared_ptr<Tag> create(const std::string& name, const TagDataType &default_type);

private:
    std::mutex _tags_mtx;
    std::map<std::string, std::shared_ptr<Tag>> _tags;

    template<typename T>
    std::shared_ptr<Tag> _create(const std::string& name, const T &default_t);
};

#endif //BROKER_BROKER_REPOSITORY_TAG_H
