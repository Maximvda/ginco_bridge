file(GLOB private_lib
    drivers/*.cpp
    threading/*.cpp
    threads/*.cpp
    app/*.cpp
    defines/*.cpp
    modules/*.cpp
    defines/protobuf/*.c
)

idf_component_register(
    SRCS
        "main.cpp"
        ${private_lib}
    INCLUDE_DIRS
        "."
        "defines"
        "defines/protobuf"
        "threading"
        "threads"
        "app"
        "drivers"
        "modules"
)
