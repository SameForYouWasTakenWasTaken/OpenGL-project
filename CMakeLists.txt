cmake_minimum_required(VERSION 3.15)
project(Game)

# Variables
set(TESTING TRUE)

option(ENABLE_IMGUI "Enable ImGui support" OFF)
option(DEBUG "Enable debug mode" ON)

# Project config
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_BUILD_TYPE Debug)

# Collect sources and headers
file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/src/*.cpp" "${CMAKE_SOURCE_DIR}/src/*.c")
file(GLOB_RECURSE HEADER_FILES CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/include/*.h" "${CMAKE_SOURCE_DIR}/include/*.hpp")
file(GLOB_RECURSE UNIT_TEST_FILES CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/unit_tests/*.cpp")

set(IMGUI_SRC
	thirdparty/imgui/imgui.cpp
	thirdparty/imgui/imgui_draw.cpp
	thirdparty/imgui/imgui_tables.cpp
	thirdparty/imgui/imgui_widgets.cpp
	thirdparty/imgui/imgui_demo.cpp
	thirdparty/imgui/backends/imgui_impl_glfw.cpp
	thirdparty/imgui/backends/imgui_impl_opengl3.cpp
)

# If CMAKE_BUILD_TYPE is not set, set it to Debug
if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

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
#catch 2 for unit testing
FetchContent_Declare(
	catch2
	GIT_REPOSITORY https://github.com/catchorg/Catch2.git
	GIT_TAG v3.11.0
)

FetchContent_MakeAvailable(glfw glm spdlog stb catch2)

set(PROJECT_INCLUDES
	"${CMAKE_SOURCE_DIR}/include"
	"${CMAKE_SOURCE_DIR}/thirdparty/imgui"
	"${CMAKE_SOURCE_DIR}/thirdparty/imgui/backends"
	${glfw_SOURCE_DIR}/include
	${stb_SOURCE_DIR}
)

set(REQUIRED_LIBRARIES glm::glm spdlog::spdlog glfw imgui)

# Add ImGui as static library
add_subdirectory(thirdparty/imgui)

# Add executable
if(ENABLE_IMGUI)
	add_definitions(-DENABLE_IMGUI)
endif()

add_executable(main ${SRC_FILES} ${HEADER_FILES} ${IMGUI_SRC})
add_executable(unit_tests ${UNIT_TEST_FILES} src/Camera.cpp)

# Include directories 

target_include_directories(main PRIVATE ${PROJECT_INCLUDES})
target_include_directories(unit_tests PRIVATE ${PROJECT_INCLUDES})


# Link libraries
target_link_libraries(main PRIVATE ${REQUIRED_LIBRARIES})

target_link_libraries(unit_tests PRIVATE ${REQUIRED_LIBRARIES} Catch2::Catch2WithMain)

if (TESTING)
	enable_testing()
	add_test(NAME RunUnitTests COMMAND unit_tests)
endif()

# Copy shaders after build
add_custom_command(TARGET main POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory
	"${CMAKE_SOURCE_DIR}/Shaders"
	$<TARGET_FILE_DIR:main>/Shaders
)
