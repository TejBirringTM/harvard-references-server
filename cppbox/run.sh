#!/usr/bin/env sh

echo "Running 'cppbox' image..."

docker run \
-v "$PROJ_DIR":/usr/src/harvard-references-server \
-p 8080:8080 \
-e PORT=8080 \
-ti cppbox:latest bash