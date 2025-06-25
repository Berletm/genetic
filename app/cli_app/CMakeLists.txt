add_executable(cli_genetic
    src/main.cpp
)

target_include_directories(cli_genetic PRIVATE include)
target_link_libraries(cli_genetic PRIVATE genetic)
