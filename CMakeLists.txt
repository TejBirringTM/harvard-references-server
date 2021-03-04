cmake_minimum_required(VERSION 3.7)
project(harvard-references-server)

set(CMAKE_CXX_STANDARD 17)

find_package(Boost COMPONENTS system filesystem regex REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

#find_package(ZLIB REQUIRED)
#include_directories(${ZLIB_INCLUDE_DIRS})

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads)

add_subdirectory(controllers)

add_executable(harvard-references-server main.cpp security/sanitizeJSON.cpp schema/verifyFields.cpp)

target_link_libraries(
        harvard-references-server
        harvard-references-server-controllers
        ${Boost_LIBRARIES}
#        ZLIB::ZLIB
        Threads::Threads
)

if(DEFINED ENV{SERVER_DEBUG})
    message("Making the DEBUG variant...")
    set(CMAKE_BUILD_TYPE Debug)
    target_compile_definitions(harvard-references-server PRIVATE SERVER_DEBUG=1)
    target_compile_definitions(harvard-references-server-controllers PRIVATE SERVER_DEBUG=1)
else()
    message("Making the PRODUCTION variant...")
endif()