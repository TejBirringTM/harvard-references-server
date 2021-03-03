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
# install cmake
RUN apt-get -qq install cmake
# install gdb (for local + remote debugging)
RUN apt-get -qq install gdb gdbserver
# install ssh (for remote debugging)
RUN apt-get -qq install openssh-server rsync vim
# install Boost C++ libs required by Crow C++ (header-only lib used by server application)
RUN apt-get -qq install libboost-system-dev libboost-filesystem-dev libboost-regex-dev
# install build-essential & Google's tcmalloc
RUN apt-get -qq install build-essential libtcmalloc-minimal4 && \
    ln -s /usr/lib/libtcmalloc_minimal.so.4 /usr/lib/libtcmalloc_minimal.so

# set password for root user
RUN echo 'root:root' | chpasswd

# set up ssh server:
RUN mkdir /var/run/sshd
# set up ssh server (sshd) configs:
RUN sed -i 's/PermitRootLogin prohibit-password/PermitRootLogin yes/' /etc/ssh/sshd_config
# ssh login fix; otherwise user is kicked off after login!
RUN sed 's@session\s*required\s*pam_loginuid.so@session optional pam_loginuid.so@g' -i /etc/pam.d/sshd

# useful example: push environment variables to Docker container
#ENV NOTVISIBLE "in users profile"
#RUN echo "export VISIBLE=now" >> /etc/profile

# expose ports:
# 8080 for server app.
# 22 for ssh server.
# 7777 for gdb server.
EXPOSE 8080 22 7777

# add username/password for "debugger" user
RUN useradd -ms /bin/bash debugger
RUN echo 'debugger:pwd' | chpasswd

# run ssh server (sshd)
CMD ["/usr/sbin/sshd", "-D"]
