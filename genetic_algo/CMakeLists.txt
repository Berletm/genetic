add_library(genetic
    src/generator.cpp
    src/selection.cpp
    src/recombination.cpp
    
    include/recombination.hpp
    include/utils.hpp
    include/generator.hpp
    include/selection.hpp
)

target_include_directories(genetic PUBLIC include)
