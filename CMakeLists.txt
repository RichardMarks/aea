cmake_minimum_required(VERSION 3.27)
project(aea C)

set(CMAKE_C_STANDARD 11)

# glad is installed in my home directory
# this should be changed to point to the glad installation for your system
set(GLAD_DIR $ENV{HOME}/glad-3.3)
add_library(GLAD ${GLAD_DIR}/src/glad.c)
target_include_directories(GLAD PRIVATE ${GLAD_DIR}/include)

# glfw3 is installed via brew package manager
find_package(glfw3 3.3 REQUIRED)

add_executable(aea_app src/main.c
        src/platform/platform.c
        src/platform/platform.h
        src/platform/internal/platform_internal.h
        src/platform/internal/platform_types.h)

target_include_directories(aea_app PRIVATE
        ${GLAD_DIR}/include
        $ENV{HOME}/stb
)

target_link_libraries(aea_app glfw GLAD dl)
