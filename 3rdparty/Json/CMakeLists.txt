cmake_minimum_required(VERSION 3.23)
project(json LANGUAGES CXX)

add_library(${PROJECT_NAME} INTERFACE)
add_library(nlohmann_json::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} INTERFACE ${PROJECT_SOURCE_DIR})
