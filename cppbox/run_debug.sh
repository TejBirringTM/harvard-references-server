#!/usr/bin/env sh

echo "Running 'cppbox' image for debug..."

# Flags required for memory profiling and native (e.g. C/C++) debugging in Docker:
# --cap-add=SYS_PTRACE
# --security-opt seccomp=unconfined

# Port mapping:
# map port 8080 (server app)
# map port 22 (ssh server) on target (container) to 7776 on host (to prevent conflict)
# map port 7777 (gdb server)

docker run \
--cap-add=SYS_PTRACE --security-opt seccomp=unconfined \
-v "$PROJ_DIR":/usr/src/harvard-references-server \
-p 8080:8080 \
-p 7776:22 \
-p 7777:7777 \
-e PORT=8080 \
-ti cppbox:latest bash