#ifndef CATEGORY_H
#define CATEGORY_H

#include <string>
#include <vector>
#include "Poco/JSON/Object.h"
#include <optional>

namespace database
{
    class Category{
        private:
            long _id;
            std::string _name_of_category;
        public:

            static Category fromJSON(const std::string & str);

            long &id();
            std::string &name_of_category();

            static void init();
            void add_category(std::string name_of_category);

            Poco::JSON::Object::Ptr toJSON() const;

    };
}

#endif