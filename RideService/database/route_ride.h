#ifndef ROUTERIDE_H
#define ROUTERIDE_H

#include <string>
#include <vector>
#include "Poco/JSON/Object.h"
#include <optional>

namespace database
{
    class RouteRide{
        private:
            long _ride_id;
            long _route_id;
        public:
	    static void init();
            static RouteRide fromJSON(const std::string & str);

            long &ride_id();
            long &route_id();
            
            void add_route_to_ride(long route_id, long ride_id);
            std::vector<RouteRide> read_all_routes_from_ride(long ride_id);

            Poco::JSON::Object::Ptr toJSON() const;

    };
}

#endif