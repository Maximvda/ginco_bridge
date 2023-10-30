file(GLOB private_lib
    drivers/*.cpp
    threading/*.cpp
    threads/*.cpp
    app/*.cpp
    defines/*.cpp
)

idf_component_register(
    SRCS
        "main.cpp"
        ${private_lib}
    INCLUDE_DIRS
        "."
        "defines"
        "threading"
        "threads"
        "app"
        "drivers"
        "modules"
        "inc"
)
