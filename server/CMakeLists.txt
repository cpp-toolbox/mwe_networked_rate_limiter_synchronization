cmake_minimum_required(VERSION 3.10)
project(server)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)


file(GLOB_RECURSE SOURCES "src/*.cpp")
# Add the main executable
add_executable(${PROJECT_NAME} ${SOURCES})
        

# Automatically generated from conan install
find_package(fmt)
find_package(enet)
target_link_libraries(${PROJECT_NAME} fmt::fmt enet::enet)
