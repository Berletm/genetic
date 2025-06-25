add_library(
    genetic
    src/generator.cpp
    
    include/utils.hpp
    include/generator.hpp
)

target_include_directories(genetic PUBLIC include)
