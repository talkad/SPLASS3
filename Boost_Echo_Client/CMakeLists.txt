cmake_minimum_required(VERSION 3.15)
project(Boost_Echo_Client)

set(CMAKE_CXX_STANDARD  14)

include_directories(include)

add_executable(Boost_Echo_Client
        include/connectionHandler.h
        src/connectionHandler.cpp
        include/UserData.h
        src/UserData.cpp
        include/Frame.h
        src/Frame.cpp
        include/StompMessagingProtocol.h
        src/StompMessagingProtocol.cpp
        include/StompEncoderDecoder.h
        src/StompEncoderDecoder.cpp
        src/Client.cpp
        )

find_package(Boost 1.56 REQUIRED COMPONENTS date_time filesystem iostreams)

target_link_libraries(Boost_Echo_Client Boost::date_time Boost::filesystem Boost::iostreams)