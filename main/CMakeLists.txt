file(GLOB private_lib modules/*.cpp drivers/*.cpp drivers/*.c drivers/protobuf/*.c)
idf_component_register(SRCS "main.cpp" ${private_lib}
                    INCLUDE_DIRS "." "inc")
