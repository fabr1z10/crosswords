include(FetchContent)

FetchContent_Declare(
        yaml-cpp
        GIT_REPOSITORY https://github.com/jbeder/yaml-cpp.git
        GIT_TAG yaml-cpp-0.7.0   # stable
)
FetchContent_MakeAvailable(yaml-cpp)


add_executable(cwcli
    main.cpp
)

target_link_libraries(cwcli
    PRIVATE
    cwcore
    yaml-cpp
)


target_compile_options(cwcli PRIVATE
    -Wall
    -Wextra
    -Wpedantic
)