add_library(genetic
    src/generator.cpp
    src/selection.cpp
    
    include/utils.hpp
    include/generator.hpp
    include/selection.hpp
)

target_include_directories(genetic PUBLIC include)
