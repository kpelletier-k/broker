#ifndef BROKER_BROKER_REPOSITORY_DB_H
#define BROKER_BROKER_REPOSITORY_DB_H

#include "broker_repository.h"
#include <sqlite.h>

class BrokerRepositoryDb : public BrokerRepository{

protected:
    explicit BrokerRepositoryDb(const std::string& name, const std::string& db_file = "");

private:
    std::unique_ptr<SQLite> _db;

    static std::unique_ptr<SQLite> _open(const std::string& name, const std::string& db_file);
};

#endif //BROKER_BROKER_REPOSITORY_DB_H
