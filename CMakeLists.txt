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

# cglm is installed via brew package manager
# but it gets installed setup for pkg-config not cmake
find_package(PkgConfig REQUIRED)
pkg_check_modules(CGLM REQUIRED IMPORTED_TARGET cglm)

add_executable(aea_app src/main.c
        src/platform/platform.c
        src/platform/platform.h
        src/platform/internal/platform_internal.h
        src/platform/internal/platform_types.h
        src/game_camera.c
        src/game_camera.h
        src/platform/effect.c
        src/platform/effect.h
        src/platform/file_utils.c
        src/platform/file_utils.h
        src/platform/mesh.c
        src/platform/mesh.h
        src/platform/shader.c
        src/platform/shader.h
)

target_include_directories(aea_app PRIVATE
        ${GLAD_DIR}/include
        $ENV{HOME}/stb
)

target_link_libraries(aea_app glfw GLAD dl ${CGLM_LIBRARIES})
target_include_directories(aea_app PUBLIC ${CGLM_INCLUDE_DIRS})
target_compile_options(aea_app PUBLIC -DDEBUG -DCGLM_DEFINE_PRINTS ${CGLM_CFLAGS_OTHER})
