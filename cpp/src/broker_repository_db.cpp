#include "../include/broker_repository_db.h"

BrokerRepositoryDb::BrokerRepositoryDb(const std::string& name, const std::string& db_file):
    BrokerRepository(name),
    _db(_open(name, db_file)){
    if (_db)
        _db->create({name, {}});
}

std::unique_ptr<SQLite> BrokerRepositoryDb::_open(const std::string& name, const std::string& db_file){
    if (db_file.empty())
        return std::make_unique<SQLite>(name + ".db");
    return std::make_unique<SQLite>(db_file);
}