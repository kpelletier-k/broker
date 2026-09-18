#ifndef BROKER_REPOSITORY_DB_SETTINGS_H
#define BROKER_REPOSITORY_DB_SETTINGS_H

#include <sqlite.h>

struct BrokerRepositoryDbSettings{
    virtual ~BrokerRepositoryDbSettings() = default;

    virtual bool is_valid() const = 0;

    virtual bool parse(const sqlite_field_t& field) = 0; // return false on error
    virtual sqlite_field_t dump() const = 0;
};

#endif //BROKER_REPOSITORY_DB_SETTINGS_H
