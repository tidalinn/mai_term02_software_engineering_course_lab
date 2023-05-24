#include <string>
#include <iostream>
#include <fstream>

#include <Poco/Data/MySQL/Connector.h>
#include <Poco/Data/MySQL/MySQLException.h>
#include <Poco/Data/SessionFactory.h>

#include <Poco/JSON/Object.h>
#include <Poco/JSON/Array.h>
#include <Poco/JSON/Parser.h>
#include <Poco/Dynamic/Var.h>

#include "./database/user.h"
#include "./database/database.h"

auto main() -> int
{

    Poco::Data::MySQL::Connector::registerConnector();

    std::string _connection_string;
    _connection_string+="host=";
    _connection_string+=std::getenv("DB_HOST");
    _connection_string+=";port=";
    _connection_string+=std::getenv("DB_PORT");
    _connection_string+=";user=";
    _connection_string+=std::getenv("DB_LOGIN");
    _connection_string+=";db=";
    _connection_string+=std::getenv("DB_DATABASE");
    _connection_string+=";password=";
    _connection_string+=std::getenv("DB_PASSWORD");
    std::cout << "connection string:" << _connection_string << std::endl;

    //get_all_hints
    std::vector<std::string> shards;
    for(size_t i=0;i<=2;++i){
        std::string shard_name = "-- sharding:";
        shard_name += std::to_string(i);
        shards.push_back(shard_name);
    }

    Poco::Data::Session session(
        Poco::Data::SessionFactory::instance().create(
            Poco::Data::MySQL::Connector::KEY, _connection_string));
    std::cout << "session created" << std::endl;
    try
    {
        std::vector<std::string> hints = shards;
        for (std::string &hint : hints){
            Poco::Data::Statement create_stmt(session);
            create_stmt << "CREATE TABLE IF NOT EXISTS `User` (`id` INT NOT NULL AUTO_INCREMENT,"
                            << "`user_id` INT NOT NULL,"
                            << "`first_name` VARCHAR(256) NOT NULL,"
                            << "`last_name` VARCHAR(256) NOT NULL,"
                            << "`login` VARCHAR(256) NOT NULL,"
                            << "`password` VARCHAR(256) NOT NULL,"
                            << "`email` VARCHAR(256) NULL,"
                            << "`phone` VARCHAR(1024) NULL,"
                            << "`socials` VARCHAR(1024) NULL,"
                            << "PRIMARY KEY (`id`),KEY `fn` (`first_name`),KEY `ln` (`last_name`));";
            create_stmt.execute();
        }
        std::cout << "table created" << std::endl;

        Poco::Data::Statement truncate_stmt(session);
        truncate_stmt << "TRUNCATE TABLE `User`;";
        truncate_stmt.execute();

        // https://www.onlinedatagenerator.com/
        std::string json;
        std::ifstream is("data.json");
        std::istream_iterator<char> eos;
        std::istream_iterator<char> iit(is);
        while (iit != eos)
            json.push_back(*(iit++));
        is.close();

        Poco::JSON::Parser parser;
        Poco::Dynamic::Var result = parser.parse(json);
        Poco::JSON::Array::Ptr arr = result.extract<Poco::JSON::Array::Ptr>();

        size_t i{0};
        for (i = 0; i < arr->size(); ++i)
        {
            Poco::JSON::Object::Ptr object = arr->getObject(i);
            std::string first_name = object->getValue<std::string>("first_name");
            std::string last_name = object->getValue<std::string>("last_name");
            std::string email = object->getValue<std::string>("email");
            std::string phone = object->getValue<std::string>("phone");
            std::string socials = object->getValue<std::string>("socials");
            std::string login = email;
            std::string password;

            long db_len = i + 1;
            // sharding_hint

            std::string key;

            key += std::to_string(db_len);

            size_t shard_number = std::hash<std::string>{}(key)%(2+1);

            std::string sharding_hint = "-- sharding:";
            sharding_hint += std::to_string(shard_number);

            std::string select_str = "INSERT INTO User (user_id, first_name, last_name, email, phone, socials, login, password) VALUES(?, ?, ?, ?, ?, ?, ?, ?)";
            select_str += sharding_hint;
            std::cout << select_str << std::endl;

            Poco::Data::Statement insert(session);
            insert << select_str,
            Poco::Data::Keywords::use(db_len),
            Poco::Data::Keywords::use(first_name),
            Poco::Data::Keywords::use(last_name),
            Poco::Data::Keywords::use(email),
            Poco::Data::Keywords::use(phone),
            Poco::Data::Keywords::use(socials),
            Poco::Data::Keywords::use(login),
            Poco::Data::Keywords::use(password);

            insert.execute();
            if(i%100==0) std::cout << ".";
        }

        std::cout << "Inserted " << i << " records" << std::endl; 
    }
    catch (Poco::Data::MySQL::ConnectionException &e)
    {
        std::cout << "connection:" << e.what() << std::endl;
    }
    catch (Poco::Data::MySQL::StatementException &e)
    {

        std::cout << "statement:" << e.what() << std::endl;
    }
    catch (std::exception* ex)
    {
        std::cout << "exception:" << ex->what() << std::endl;
    }
    return 1;
}