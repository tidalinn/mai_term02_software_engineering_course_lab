#include "ride.h"
#include "database.h"
#include "../config/config.h"

#include <Poco/Data/MySQL/Connector.h>
#include <Poco/Data/MySQL/MySQLException.h>
#include <Poco/Data/SessionFactory.h>
#include <Poco/Data/RecordSet.h>
#include <Poco/JSON/Parser.h>
#include <Poco/Dynamic/Var.h>

#include <sstream>
#include <exception>

using namespace Poco::Data::Keywords;
using Poco::Data::Session;
using Poco::Data::Statement;

namespace database
{

    void Ride::init()
    {
        try
        {
            Poco::Data::Session session = database::Database::get().create_session();
            Statement create_stmt(session);
            create_stmt << "CREATE TABLE IF NOT EXISTS `Ride`(`id` int NOT NULL AUTO_INCREMENT,"
                        << "`ride_name` varchar(1024) NOT NULL,"
                        << "`creator_id` int NOT NULL,"
                        << "`category_id` int NOT NULL,"
                        << "`description` text NOT NULL,"
                        << "`ride_date` text NOT NULL,"
                        << "`total_followers` int NOT NULL,"

                        << "PRIMARY KEY (`id`),"
                        << "KEY `FK_2` (`category_id`),"
                        << "CONSTRAINT `FK_1` FOREIGN KEY `FK_2` (`category_id`) REFERENCES `Category` (`id`));",
                now;
        }

        catch (Poco::Data::MySQL::ConnectionException &e)
        {
            std::cout << "connection:" << e.what() << std::endl;
            throw;
        }
        catch (Poco::Data::MySQL::StatementException &e)
        {
            std::cout << "statement:" << e.what() << std::endl;
            throw;
        }
    }

    Poco::JSON::Object::Ptr Ride::toJSON() const
    {
        Poco::JSON::Object::Ptr root = new Poco::JSON::Object();

        root->set("id", _id);
        root->set("ride_name", _ride_name);
        root->set("creator_id", _creator_id);
        root->set("category_id", _category_id);
        root->set("description", _description);
        root->set("ride_date", _ride_date);
        root->set("total_followers", _total_followers);

        return root;
    }

    Ride Ride::fromJSON(const std::string &str)
    {
        Ride ride;
        Poco::JSON::Parser parser;
        Poco::Dynamic::Var result = parser.parse(str);
        Poco::JSON::Object::Ptr object = result.extract<Poco::JSON::Object::Ptr>();

        ride.id() = object->getValue<long>("id");
        ride.ride_name() = object->getValue<std::string>("ride_name");
        ride.creator_id() = object->getValue<long>("creator_id");
        ride.category_id() = object->getValue<long>("category_id");
        ride.description() = object->getValue<std::string>("description");
        ride.ride_date() = object->getValue<std::string>("ride_date");
        ride.total_followers() = object->getValue<std::string>("total_followers");

        return ride;
    }


    void Ride::add_ride(std::string ride_name, long creator_id, long category_id, std::string description, std::string ride_date, long total_followers)
    {

        try
        {
            Poco::Data::Session session = database::Database::get().create_session();
            Poco::Data::Statement insert(session);

            insert << "INSERT INTO Ride (ride_name, creator_id, category_id, description, ride_date, total_followers) VALUES(?, ?, ?, ?, ?, ?)",
                use(ride_name),
                use(creator_id),
                use(category_id),
                use(description),
                use(ride_date);
                use(total_followers);

            insert.execute();

            std::cout << "inserted:" << _id << std::endl;
        }
        catch (Poco::Data::MySQL::ConnectionException &e)
        {
            std::cout << "connection:" << e.what() << std::endl;
            throw;
        }
        catch (Poco::Data::MySQL::StatementException &e)
        {
            std::cout << "statement:" << e.what() << std::endl;
            throw;
        }
    }

    long &Ride::id()
    {
        return _id;
    }

    std::string &Ride::ride_name()
    {
        return _ride_name;
    }

    long &Ride::creator_id()
    {
        return _creator_id;
    }

    long &Ride::category_id()
    {
        return _category_id;
    }
    
    std::string &Ride::description()
    {
        return _description;
    }

    std::string &Ride::ride_date()
    {
        return _ride_date;
    }

    std::string &Ride::total_followers()
    {
        return _total_followers;
    }
}