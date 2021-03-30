#ifndef DATABASE_H
#define DATABASE_H

#include <string>
#include <Poco/Data/MySQL/Connector.h>
#include <Poco/Data/MySQL/MySQLException.h>
#include <Poco/Data/SessionFactory.h>

namespace database{
    class Database{
        private:
            std::string _connection_string_read;
            std::string _connection_string_write;
            Database();
        public:
            static Database& get();
            Poco::Data::Session create_session_read();
            Poco::Data::Session create_session_write();
    };
}
#endif