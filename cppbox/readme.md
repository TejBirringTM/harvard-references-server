#### (1) Build Image
`$ docker build -t cppbox .`

#### (2) Run BASH shell
```
$ export PROJ_DIR=<harvard-references-server root directory>

$ docker run \
-v "$PROJ_DIR":/usr/src/harvard-references-server \
-p 8080:8080 \
-e PORT=8080 \
-ti cppbox:latest bash
```

Alternatively, use the scripts provided:
```
# For Building/Testing
$ ./run.sh

# For Debugging (using GDB)
$ ./run_debug.sh
```

To open a terminal in existing container:
```
$ docker exec -it <container name> bash
```