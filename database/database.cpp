#include "database.h"
#include "../config/config.h"

namespace database{
    Database::Database(){
        _connection_string+="host=";
        _connection_string+=Config::get().get_host();
        _connection_string+=";user=";
        _connection_string+=Config::get().get_login();
        _connection_string+=";db=";
        _connection_string+=Config::get().get_database();
        _connection_string+=";password=";
        _connection_string+=Config::get().get_password();

        Poco::Data::MySQL::Connector::registerConnector();
    }

    Database& Database::get(){
        static Database _instance;
        return _instance;
    }

    Poco::Data::Session Database::create_session(){
        return Poco::Data::Session(Poco::Data::SessionFactory::instance().create(Poco::Data::MySQL::Connector::KEY, _connection_string));
    }

}