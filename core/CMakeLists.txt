add_library(cwcore STATIC src/grid.cpp src/dict.cpp src/decision.cpp src/solver.cpp src/random.cpp)

target_include_directories(cwcore
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)