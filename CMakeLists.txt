cmake_minimum_required(VERSION 3.21)
project(RogueLoot VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(external/CommonLibSSE-NG)

add_library(${PROJECT_NAME} SHARED
    src/main.cpp
    src/Hooks.cpp
    src/LootConfig.cpp
)

target_link_libraries(${PROJECT_NAME} 
    PRIVATE 
        CommonLibSSE::CommonLibSSE
)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_23)

set_target_properties(${PROJECT_NAME} PROPERTIES
    OUTPUT_NAME "RogueLoot"
)