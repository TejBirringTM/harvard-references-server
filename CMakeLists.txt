cmake_minimum_required(VERSION 3.7)
project(harvard-references-server)

set(CMAKE_CXX_STANDARD 17)
set(THREADS_PREFER_PTHREAD_FLAG ON)

find_package(Boost COMPONENTS system filesystem regex REQUIRED)
find_package(Threads)

include_directories(${Boost_INCLUDE_DIRS})
add_executable(harvard-references-server main.cpp)
add_subdirectory(controllers)
target_link_libraries(
        harvard-references-server
        harvard-references-server-controllers
        ${Boost_LIBRARIES}
        Threads::Threads)

target_compile_definitions(harvard-references-server PRIVATE SERVER_DEBUG=1)
target_compile_definitions(harvard-references-server-controllers PRIVATE SERVER_DEBUG=1)
