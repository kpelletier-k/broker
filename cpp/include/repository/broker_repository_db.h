#ifndef BROKER_BROKER_REPOSITORY_DB_H
#define BROKER_BROKER_REPOSITORY_DB_H

#include "broker_repository.h"
#include "broker_repository_db_settings.h"
#include <sqlite_create.h>

#define BR_REPO_NAME "repo"
#define BR_PARAMS_NAME "params"
#define BR_SETTINGS_NAME "settings"

template<typename SETTINGS>
class BrokerRepositoryDb : public BrokerRepository{

protected:
    explicit BrokerRepositoryDb(const std::string& name,
                                const std::string& db_file = "",
                                const std::list<SQLiteCreate::Field> &fields = {},
                                const std::map<std::string, CmdCallFnc>& callers = {},
                                const std::list<std::shared_ptr<Tag>>& tags = {});

    explicit BrokerRepositoryDb(const std::string& name,
                                const std::shared_ptr<SQLite>& db,
                                const std::list<SQLiteCreate::Field> &fields = {},
                                const std::map<std::string, CmdCallFnc>& callers = {},
                                const std::list<std::shared_ptr<Tag>>& tags = {});


    void _registered() override;
    virtual void _registered(const SETTINGS& settings) = 0;

    virtual SETTINGS _settings_null() = 0;// return new settings or null
    virtual SETTINGS _settings_duplication(const std::list<SETTINGS>& settings_list) = 0;// return selected settings

    std::shared_ptr<SQLite> _db;

private:
    static std::shared_ptr<SQLite> _open(const std::string& name, const std::string& db_file);
};


template<typename SETTINGS>
BrokerRepositoryDb<SETTINGS>::BrokerRepositoryDb(const std::string& name,
                                        const std::string& db_file,
                                        const std::list<SQLiteCreate::Field> &fields,
                                        const std::map<std::string, CmdCallFnc>& callers,
                                        const std::list<std::shared_ptr<Tag>>& tags):
    BrokerRepositoryDb(name, _open(name, db_file), fields, callers, tags) {}

template<typename SETTINGS>
BrokerRepositoryDb<SETTINGS>::BrokerRepositoryDb(const std::string& name,
                                        const std::shared_ptr<SQLite>& db,
                                        const std::list<SQLiteCreate::Field> &fields,
                                        const std::map<std::string, CmdCallFnc>& callers,
                                        const std::list<std::shared_ptr<Tag>>& tags):
    BrokerRepository(name, callers, tags),
    _db(db){
    if (_db){
        _db->create({BR_SETTINGS_NAME, {
            SQLiteCreate::Field(BR_REPO_NAME , SqliteFieldType::string_type),
            SQLiteCreate::Field(BR_PARAMS_NAME),
        }});
        if (!fields.empty())
            _db->create({name, fields});
    }
}


template<typename SETTINGS>
void BrokerRepositoryDb<SETTINGS>::_registered(){
    const auto s = _db->select(SQLiteSelect(BR_SETTINGS_NAME, SQLiteWhere(BR_REPO_NAME, name)), false);
    if (s.empty()){
        auto new_s = _settings_null();
        if (_db->insert(BR_SETTINGS_NAME, {{BR_REPO_NAME, name},{BR_PARAMS_NAME, new_s.dump()}}))
            _registered(new_s);
        //else //todo error
    }
    // else if (s.size() > 1)
    //     _settings_duplication(); //todo
    else{
        SETTINGS new_s;
        if (new_s.parse(s.front().at("params")))
            _registered(new_s);
        // toto else error
    }
}

template<typename SETTINGS>
std::shared_ptr<SQLite>  BrokerRepositoryDb<SETTINGS>::_open(const std::string& name, const std::string& db_file){
    if (db_file.empty())
        return std::make_shared<SQLite>(name + ".db");
    return std::make_shared<SQLite>(db_file);
}

#endif //BROKER_BROKER_REPOSITORY_DB_H
