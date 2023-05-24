#include "route_ride.h"
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

    void RouteRide::init()
    {
        try
        {

            Poco::Data::Session session = database::Database::get().create_session();
            Statement create_stmt(session);
            create_stmt <<"CREATE TABLE IF NOT EXISTS `RouteRide`(`route_id` int NOT NULL ,"
                        <<"`ride_id`   int NOT NULL ,"

                        <<"PRIMARY KEY (`route_id`, `ride_id`),"
                        <<"KEY `FK_1` (`route_id`),"
                        <<"CONSTRAINT `FK_4` FOREIGN KEY `FK_1` (`route_id`) REFERENCES `Route` (`id`),"
                        <<"KEY `FK_3` (`ride_id`),"
                        <<"CONSTRAINT `FK_5` FOREIGN KEY `FK_3` (`ride_id`) REFERENCES `Ride` (`id`));",
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

    Poco::JSON::Object::Ptr RouteRide::toJSON() const
    {
        Poco::JSON::Object::Ptr root = new Poco::JSON::Object();

        root->set("route_id", _route_id);
        root->set("ride_id", _ride_id);

        return root;
    }

    RouteRide RouteRide::fromJSON(const std::string &str)
    {
        RouteRide routeride;
        Poco::JSON::Parser parser;
        Poco::Dynamic::Var result = parser.parse(str);
        Poco::JSON::Object::Ptr object = result.extract<Poco::JSON::Object::Ptr>();

        routeride.route_id() = object->getValue<long>("route_id");
        routeride.ride_id() = object->getValue<long>("ride_id");

        return routeride;
    }

    void RouteRide::add_Route_to_ride(long route_id, long ride_id)
    {
        try
        {
            Poco::Data::Session session = database::Database::get().create_session();
            Poco::Data::Statement insert(session);

            insert << "INSERT INTO RouteRide (route_id, ride_id) VALUES(?, ?)",
                use(route_id),
                use(ride_id);

            insert.execute();

            std::cout << "inserted: " << _route_id << " & " << _ride_id << std::endl;
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

    std::vector<RouteRide> RouteRide::read_all_Routes_from_ride(long ride_id)
    {
        try
        {
            Poco::Data::Session session = database::Database::get().create_session();
            Statement select(session);
            std::vector<RouteRide> result;
            RouteRide a;
            select << "SELECT route_id, ride_id FROM RouteRide where ride_id=?",
                into(a._route_id),
                use(ride_id),
                range(0, 1); //  iterate over result set one row at a time

            while (!select.done())
            {
                if (select.execute())
                    result.push_back(a);
            }
            return result;
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

    long &RouteRide::route_id()
    {
        return _route_id;
    }

    long &RouteRide::ride_id()
    {
        return _ride_id;
    }

}