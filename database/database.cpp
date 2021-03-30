#include "database.h"
#include "../config/config.h"

namespace database{
    Database::Database(){
        _connection_string_read+="host=";
        _connection_string_read+=Config::get().get_read_request_ip();
        _connection_string_read+=";user=";
        _connection_string_read+=Config::get().get_login();
        _connection_string_read+=";db=";
        _connection_string_read+=Config::get().get_database();
        _connection_string_read+=";password=";
        _connection_string_read+=Config::get().get_password();


        _connection_string_write+="host=";
        _connection_string_write+=Config::get().get_write_request_ip();
        _connection_string_write+=";user=";
        _connection_string_write+=Config::get().get_login();
        _connection_string_write+=";db=";
        _connection_string_write+=Config::get().get_database();
        _connection_string_write+=";password=";
        _connection_string_write+=Config::get().get_password();

        Poco::Data::MySQL::Connector::registerConnector();
    }

    Database& Database::get(){
        static Database _instance;
        return _instance;
    }

    Poco::Data::Session Database::create_session_read(){
        return Poco::Data::Session(Poco::Data::SessionFactory::instance().create(Poco::Data::MySQL::Connector::KEY, _connection_string_read ));
    }

    Poco::Data::Session Database::create_session_write(){
        return Poco::Data::Session(Poco::Data::SessionFactory::instance().create(Poco::Data::MySQL::Connector::KEY, _connection_string_write ));
    }
}