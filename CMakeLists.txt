cmake_minimum_required(VERSION 3.2)

project(hl_server C CXX)

SET (EXAMPLE_BINARY "hl_mai_lab_01")
SET (OTHER_SERVICE "other_service")

find_package(OpenSSL)
find_package(Threads)
find_package(ZLIB)
find_package(Poco REQUIRED COMPONENTS Foundation Util Net XML JSON Crypto NetSSL)


if(NOT ${Poco_FOUND})
    message(FATAL_ERROR "Poco C++ Libraries not found.")
endif()

include_directories(${Poco_INCLUDE_DIRS})
include_directories("/usr/include/mysql")
include_directories("/usr/local/include/mysql")
link_directories("/usr/local/lib")

SET(CMAKE_CXX_STANDARD 17)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(${EXAMPLE_BINARY} main.cpp 
                                 config/config.cpp 
                                 database/database.cpp
                                 database/user.cpp)


target_include_directories(${EXAMPLE_BINARY} PRIVATE "${CMAKE_BINARY_DIR}")
target_compile_options(${EXAMPLE_BINARY} PRIVATE -Wall -Wextra -pedantic -Werror )
set_target_properties(${EXAMPLE_BINARY} PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(${EXAMPLE_BINARY} PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED ON)

target_link_libraries(${EXAMPLE_BINARY} PRIVATE 
                             ${CMAKE_THREAD_LIBS_INIT} 
                             ${Poco_LIBRARIES}
                             "PocoData"
                             "PocoDataMySQL"
                             "mysqlclient"
                             ZLIB::ZLIB)




add_executable(${OTHER_SERVICE} other_main.cpp)
target_include_directories(${OTHER_SERVICE} PRIVATE "${CMAKE_BINARY_DIR}")
target_compile_options(${OTHER_SERVICE} PRIVATE -Wall -Wextra -pedantic -Werror )
set_target_properties(${OTHER_SERVICE} PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(${OTHER_SERVICE} PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED ON)

target_link_libraries(${OTHER_SERVICE} PRIVATE 
                             ${CMAKE_THREAD_LIBS_INIT} 
                             ${Poco_LIBRARIES}
                             "PocoData"
                             "PocoDataMySQL"
                             "mysqlclient"
                             ZLIB::ZLIB)





