#ifndef USEHANDLER_H
#define USEHANDLER_H

#include "Poco/Net/HTTPServer.h"
#include "Poco/Net/HTTPRequestHandler.h"
#include "Poco/Net/HTTPRequestHandlerFactory.h"
#include "Poco/Net/HTTPServerParams.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"
#include "Poco/Net/HTTPServerParams.h"
#include "Poco/Net/HTMLForm.h"
#include "Poco/Net/ServerSocket.h"
#include "Poco/Timestamp.h"
#include "Poco/DateTimeFormatter.h"
#include "Poco/DateTimeFormat.h"
#include "Poco/Exception.h"
#include "Poco/ThreadPool.h"
#include "Poco/Util/ServerApplication.h"
#include "Poco/Util/Option.h"
#include "Poco/Util/OptionSet.h"
#include "Poco/Util/HelpFormatter.h"
#include <iostream>
#include <iostream>
#include <fstream>

using Poco::DateTimeFormat;
using Poco::DateTimeFormatter;
using Poco::ThreadPool;
using Poco::Timestamp;
using Poco::Net::HTMLForm;
using Poco::Net::HTTPRequestHandler;
using Poco::Net::HTTPRequestHandlerFactory;
using Poco::Net::HTTPServer;
using Poco::Net::HTTPServerParams;
using Poco::Net::HTTPServerRequest;
using Poco::Net::HTTPServerResponse;
using Poco::Net::NameValueCollection;
using Poco::Net::ServerSocket;
using Poco::Util::Application;
using Poco::Util::HelpFormatter;
using Poco::Util::Option;
using Poco::Util::OptionCallback;
using Poco::Util::OptionSet;
using Poco::Util::ServerApplication;

#include "../database/ride.h"
#include "../database/category.h"
#include "../database/route_ride.h"
#include "../../helper.h"

static bool hasSubstr(const std::string &str, const std::string &substr)
{
    if (str.size() < substr.size())
        return false;
    for (size_t i = 0; i <= str.size() - substr.size(); ++i)
    {
        bool ok{true};
        for (size_t j = 0; ok && (j < substr.size()); ++j)
            ok = (str[i + j] == substr[j]);
        if (ok)
            return true;
    }
    return false;
}

class RideHandler : public HTTPRequestHandler
{
public:
    RideHandler(const std::string &format) : _format(format)
    {
    }

    void handleRequest(HTTPServerRequest &request,
                       HTTPServerResponse &response)
    {
        HTMLForm form(request, request.stream());

        try
        {
            std::string scheme;
            std::string info;
            request.getCredentials(scheme, info);
            std::string login, password, url;
            get_identity(info, login, password);
            url = "http://" + host+":8080/auth";
            if (do_get(url, login, password))
            {
                if (hasSubstr(request.getURI(), "/add_category") &&
                    (request.getMethod() == Poco::Net::HTTPRequest::HTTP_POST) && form.has("name_of_category"))
                {               
                    std::string name_of_category = form.get("name_of_category").c_str();

                    database::Category result;
                    result.add_category(name_of_category);

                    response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
                    response.setChunkedTransferEncoding(true);
                    response.setContentType("application/json");
                    std::ostream &ostr = response.send();
                    Poco::JSON::Stringifier::stringify(result.toJSON(), ostr);
                    return;
                }
                
                else if (hasSubstr(request.getURI(), "/add_ride") &&
                    (request.getMethod() == Poco::Net::HTTPRequest::HTTP_POST)&&
                    form.has("ride_name")&&
                    form.has("creator_id")&&
                    form.has("category_id")&&
                    form.has("description")&&
                    form.has("ride_date")&&
                    form.has("total_followers"))
                {
                    std::string ride_name = form.get("ride_name").c_str();
                    long creator_id = atol(form.get("creator_id").c_str());
                    long category_id = atol(form.get("category_id").c_str());
                    std::string description = form.get("description").c_str();
                    std::string ride_date = form.get("ride_date").c_str();
                    long total_followers = atol(form.get("total_followers").c_str());
            
                    database::Ride ride;
                    ride.add_ride(ride_name, creator_id, category_id, description, ride_date, total_followers);

                    response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
                    response.setChunkedTransferEncoding(true);
                    response.setContentType("application/json");
                    response.send();
                    return;
                }
                if (hasSubstr(request.getURI(), "/add_route_to_ride") &&
                    (request.getMethod() == Poco::Net::HTTPRequest::HTTP_POST) &&
                    form.has("route_id") &&
                    form.has("ride_id"))
                {
                    long route_id = atol(form.get("route_id").c_str());
                    long ride_id = atol(form.get("ride_id").c_str());

                    database::RouteRide routeride;
                    routeride.add_route_to_ride(route_id, ride_id);

                    response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
                    response.setChunkedTransferEncoding(true);
                    response.setContentType("application/json");
                    std::ostream &ostr = response.send();
                    Poco::JSON::Stringifier::stringify(routeride.toJSON(), ostr);
                    return;
                }
                else if (hasSubstr(request.getURI(), "/read_all_routes_from_ride") &&
                        (request.getMethod() == Poco::Net::HTTPRequest::HTTP_GET)&&
                        form.has("ride_id"))
                {
                    long ride_id = atol(form.get("ride_id").c_str());

                    database::RouteRide routeride;
                    auto results = routeride.read_all_routes_from_ride(ride_id);

                    Poco::JSON::Array arr;
                    for (auto s : results)
                        arr.add(s.toJSON());
                    response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
                    response.setChunkedTransferEncoding(true);
                    response.setContentType("application/json");
                    std::ostream &ostr = response.send();
                    Poco::JSON::Stringifier::stringify(arr, ostr);

                    return;
                }
            }
            
        }
        catch (...)
        {
        }

        response.setStatus(Poco::Net::HTTPResponse::HTTPStatus::HTTP_NOT_FOUND);
        response.setChunkedTransferEncoding(true);
        response.setContentType("application/json");
        Poco::JSON::Object::Ptr root = new Poco::JSON::Object();
        root->set("type", "/errors/not_found");
        root->set("phone", "Internal exception");
        root->set("status", Poco::Net::HTTPResponse::HTTPStatus::HTTP_NOT_FOUND);
        root->set("detail", "request ot found");
        root->set("instance", "/ride");
        std::ostream &ostr = response.send();
        Poco::JSON::Stringifier::stringify(root, ostr);
    }

private:
    std::string _format;
};
#endif