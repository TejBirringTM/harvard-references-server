cmake_minimum_required(VERSION 3.7)
project(harvard-references-server)

# C++ settings
set(CMAKE_CXX_STANDARD 17)

# Find packages...
## Find BOOST
find_package(Boost COMPONENTS system filesystem regex REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})
## Find ZLIB (used for HTTP response compression)
#find_package(ZLIB REQUIRED)
#include_directories(${ZLIB_INCLUDE_DIRS})
# Posix Multi-threading (PThreads) API implementation
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads)

# Add controllers library (PRODUCTION + DEBUG)
add_subdirectory(src/controllers)

# Add executable - PRODUCTION
add_executable(harvard-references-server src/main.cpp src/security/sanitizeJSON.cpp src/schema/verifyFields.cpp)
target_link_libraries(
        harvard-references-server
        harvard-references-server-controllers
        ${Boost_LIBRARIES}
#        ZLIB::ZLIB
        Threads::Threads
)

# Add executable - DEBUG
add_executable(harvard-references-server-debug src/main.cpp src/security/sanitizeJSON.cpp src/schema/verifyFields.cpp)
target_compile_definitions(harvard-references-server-debug PRIVATE SERVER_DEBUG=1)
target_link_libraries(
        harvard-references-server-debug
        harvard-references-server-controllers-debug
        ${Boost_LIBRARIES}
        #        ZLIB::ZLIB
        Threads::Threads
)

#
#if(DEFINED ENV{SERVER_DEBUG})
#    message("Making the DEBUG variant...")
#    set(CMAKE_BUILD_TYPE Debug)
#
#
#else()
#    message("Making the PRODUCTION variant...")
#endif()