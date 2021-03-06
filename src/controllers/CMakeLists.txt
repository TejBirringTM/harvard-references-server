# PRODUCTION variant
add_library(
        harvard-references-server-controllers
        respondToGET.cpp
        initialize.cpp
)

# DEBUG variant
add_library(
        harvard-references-server-controllers-debug
        respondToGET.cpp
        initialize.cpp
)
target_compile_definitions(harvard-references-server-controllers-debug PRIVATE SERVER_DEBUG=1)