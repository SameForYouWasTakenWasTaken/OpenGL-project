cmake_minimum_required(VERSION 3.15)
project(Game)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

# Collect sources and headers
file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/src/*.cpp" "${CMAKE_SOURCE_DIR}/src/*.c")
file(GLOB_RECURSE HEADER_FILES CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/include/*.h" "${CMAKE_SOURCE_DIR}/include/*.hpp")

include(FetchContent)

# GLFW
FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.4
)

# GLM
FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG master
)
# spdlog for logging
FetchContent_Declare(
    spdlog
    GIT_REPOSITORY https://github.com/gabime/spdlog.git
    GIT_TAG v1.15.3
)
# stb for textures
FetchContent_Declare(
    stb
    GIT_REPOSITORY https://github.com/nothings/stb.git
)

FetchContent_MakeAvailable(glfw glm spdlog stb)

# Add executable
add_executable(main ${SRC_FILES} ${HEADER_FILES})
target_include_directories(main PRIVATE "${CMAKE_SOURCE_DIR}/include")
target_link_libraries(main PRIVATE glm::glm glfw spdlog::spdlog)

add_custom_command(TARGET main POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_SOURCE_DIR}/Shaders"
    $<TARGET_FILE_DIR:main>/Shaders
)
