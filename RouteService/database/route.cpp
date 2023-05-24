#include "route.h"
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

    void Route::init()
    {
        try
        {

            Poco::Data::Session session = database::Database::get().create_session();
            Statement create_stmt(session);
            create_stmt << "CREATE TABLE IF NOT EXISTS `Route`(`id` int NOT NULL AUTO_INCREMENT,"
                        << "`route_name` text NOT NULL ,"
                        << "`creator_id` int NOT NULL ,"
                        << "`point_start` text NOT NULL ,"
                        << "`point_end` text NOT NULL ,"
                        << "`point_other` text NOT NULL ,"

                        << "PRIMARY KEY (`id`));",
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

    Poco::JSON::Object::Ptr Route::toJSON() const
    {
        Poco::JSON::Object::Ptr root = new Poco::JSON::Object();

        root->set("id", _id);
        root->set("route_name", _route_name);
        root->set("creator_id", _creator_id);
        root->set("point_start", _point_start);
        root->set("point_end", _point_end);
        root->set("point_other", _point_other);

        return root;
    }

    Route Route::fromJSON(const std::string &str)
    {
        Route route;
        Poco::JSON::Parser parser;
        Poco::Dynamic::Var result = parser.parse(str);
        Poco::JSON::Object::Ptr object = result.extract<Poco::JSON::Object::Ptr>();

        route.id() = object->getValue<long>("id");
        route.route_name() = object->getValue<std::string>("route_name");
        route.creator_id() = object->getValue<long>("creator_id");
        route.point_start() = object->getValue<std::string>("point_start");
        route.point_end() = object->getValue<std::string>("point_end");
        route.point_other() = object->getValue<std::string>("point_other");

        return route;
    }

    std::vector<Route> Route::read_all_routes()
    {
        try
        {
            Poco::Data::Session session = database::Database::get().create_session();
            Statement select(session);
            std::vector<Route> result;
            Route a;
            select << "SELECT id, route_name, creator_id, point_start, point_end, point_other FROM `Route`",
                into(a._id),
                into(a._route_name),
                into(a._creator_id),
                into(a._point_start),
                into(a._point_end),
                into(a._point_other),
                range(0, 1); //  iterate over result set one row at a time
            
            while (!select.done())
            {
                if (select.execute()) {
                    result.push_back(a);
                }
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

    std::optional<Route> Route::search_route(long id)
    {
        try
        {
            Poco::Data::Session session = database::Database::get().create_session();
            Poco::Data::Statement select(session);
            Route a;
            select << "SELECT id, route_name, creator_id, point_start, point_end, point_other FROM Route where id=?",
                into(a._id),
                into(a._route_name),
                into(a._creator_id),
                into(a._point_start),
                into(a._point_end),
                into(a._point_other),
                use(id),
                range(0, 1); //  iterate over result set one row at a time
 
            select.execute();
            Poco::Data::RecordSet rs(select);
            if (rs.moveFirst()) return a;
        }

        catch (Poco::Data::MySQL::ConnectionException &e)
        {
            std::cout << "connection:" << e.what() << std::endl;
        }
        catch (Poco::Data::MySQL::StatementException &e)
        {
            std::cout << "statement:" << e.what() << std::endl;   
        }
        return {};
    }

    void Route::add_route(std::string route_name, long creator_id, std::string point_start, std::string point_end, std::string point_other)
    {

        try
        {
            Poco::Data::Session session = database::Database::get().create_session();
            Poco::Data::Statement insert(session);

            insert << "INSERT INTO Route (route_name, creator_id, point_start, point_end, point_other) VALUES(?, ?, ?, ?, ?)",
                use(route_name),
                use(creator_id),
                use(point_start),
                use(point_end),
                use(point_other);

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

    long &Route::id()
    {
        return _id;
    }

    std::string &Route::route_name()
    {
        return _route_name;
    }

    long &Route::creator_id()
    {
        return _creator_id;
    }
    
    std::string &Route::point_start()
    {
        return _point_start;
    }

    std::string &Route::point_end()
    {
        return _point_end;
    }

    std::string &Route::point_other()
    {
        return _point_other;
    }
}