add_library(genetic
    src/generator.cpp
    src/selection.cpp
    src/recombination.cpp
    src/mutation.cpp
    src/evolution.cpp
    
    include/evolution.hpp
    include/mutation.hpp
    include/recombination.hpp
    include/utils.hpp
    include/generator.hpp
    include/selection.hpp
)

target_include_directories(genetic PUBLIC include)
