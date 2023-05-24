#ifndef RIDE_H
#define RIDE_H

#include <string>
#include <vector>
#include "Poco/JSON/Object.h"
#include <optional>

namespace database
{
    class Ride{
        private:
            long _id;
            std::string _ride_name;
            long _creator_id;
            long _category_id;
            std::string _description;
            std::string _ride_date;
            long _total_followers;

        public:
            static void init();
            static Ride fromJSON(const std::string & str);

            long &id();
            std::string &ride_name();
            long &creator_id();
            long &category_id();
            std::string &description();
            std::string &ride_date();
            long &total_followers();

            void add_ride(std::string ride_name, long creator_id, long category_id, std::string description, std::string ride_date, long total_followers);

            Poco::JSON::Object::Ptr toJSON() const;

    };
}

#endif