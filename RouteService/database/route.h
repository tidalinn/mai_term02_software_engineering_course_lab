#ifndef ROUTE_H
#define ROUTE_H

#include <string>
#include <vector>
#include "Poco/JSON/Object.h"
#include <optional>

namespace database
{
    class Route{
        private:
            long _id;
            std::string _route_name;
            long _creator_id;
            std::string _point_start;
            std::string _point_end;
            std::string _point_other;

        public:
            static Route fromJSON(const std::string & str);

            long& id();
            std::string &route_name();
            long& creator_id();
            std::string &point_start();
            std::string &point_end();
            std::string &point_other();

            static void init();
            std::vector<Route> read_all_routes();
            std::optional<Route> search_route(long id);
            void add_route(std::string route_name, long creator_id, std::string point_start, std::string point_end, std::string point_other);

            Poco::JSON::Object::Ptr toJSON() const;

    };
}

#endif