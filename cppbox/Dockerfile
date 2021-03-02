##
##  This Dockerfile specifies an image for
##  developing, testing, building, and debugging
##  the server application.
##

# Use Docker's official GCC image for GCC version 10.2.0
FROM gcc:10.2.0

# update packages list
RUN apt-get -qq update
# upgrade out-of-date packages
RUN apt-get -qq upgrade
# install gdb
RUN apt-get -qq install gdb
# install cmake
RUN apt-get -qq install cmake
# install Boost C++ libs required by Crow C++ (header-only lib used by server application)
RUN apt-get -qq install libboost-system-dev libboost-filesystem-dev libboost-regex-dev
# install build-essential & Google's tcmalloc
RUN apt-get -qq install build-essential libtcmalloc-minimal4 && \
    ln -s /usr/lib/libtcmalloc_minimal.so.4 /usr/lib/libtcmalloc_minimal.so
