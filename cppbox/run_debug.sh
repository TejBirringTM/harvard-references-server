#!/usr/bin/env sh

echo "Running 'cppbox' image for debug..."

# Flags required for memory profiling and native (e.g. C/C++) debugging in Docker:
# --cap-add=SYS_PTRACE
# --security-opt seccomp=unconfined

docker run \
--cap-add=SYS_PTRACE --security-opt seccomp=unconfined \
-v "$PROJ_DIR":/usr/src/harvard-references-server \
-p 8080:8080 \
-e PORT=8080 \
-ti cppbox:latest bash