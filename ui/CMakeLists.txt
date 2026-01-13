include(FetchContent)

FetchContent_Declare(
        yaml-cpp
        GIT_REPOSITORY https://github.com/jbeder/yaml-cpp.git
        GIT_TAG yaml-cpp-0.7.0   # stable
)
FetchContent_MakeAvailable(yaml-cpp)

project(Cruciverba LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_AUTOMOC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

set(SOURCES main.cpp mainwindow.cpp crossword.cpp gridselect.cpp)

qt_add_executable(Cruciverba ${SOURCES})

target_link_libraries(Cruciverba PRIVATE Qt6::Widgets cwcore yaml-cpp)

# Copy assets folder next to the executable
add_custom_command(TARGET Cruciverba POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/assets"
        "$<TARGET_FILE_DIR:Cruciverba>/assets"
)