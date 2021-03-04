# PRODUCTION variant
add_library(
        harvard-references-server-controllers
        controllers.cpp
)

# DEBUG variant
add_library(
        harvard-references-server-controllers-debug
        controllers.cpp
)
target_compile_definitions(harvard-references-server-controllers-debug PRIVATE SERVER_DEBUG=1)