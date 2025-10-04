cmake_minimum_required(VERSION 3.5.0)
project(GameEngine VERSION 1.0.0)
cmake_policy(SET CMP0072 NEW)

find_package(OpenGL REQUIRED)
find_package(OpenCL REQUIRED)

add_executable(GameEngine
	# User Interface
	srcs/main.cpp

	# Framework Classes
	framework/core/Logger.cpp
	framework/classes/ParticleSystem.cpp
	framework/classes/Shader.cpp
	framework/classes/Window.cpp

	# Dependencies
	dependencies/glad/glad.c
	dependencies/glfw/libglfw3.a
	dependencies/stb_image/stb_def.c
)

target_include_directories(GameEngine PRIVATE
	${CMAKE_SOURCE_DIR}/includes
	${CMAKE_SOURCE_DIR}/dependencies
	${CMAKE_SOURCE_DIR}/framework
	${CMAKE_SOURCE_DIR}/framework/classes
	${CMAKE_SOURCE_DIR}/framework/core
	${OpenCL_INCLUDE_DIRS}
)

target_link_libraries(GameEngine PRIVATE
	OpenGL::GL
	glfw
	${OpenCL_LIBRARIES}
)

set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -fsanitize=address -lasan" CACHE STRING "Debug build flags" FORCE)
set(CMAKE_CXX_FLAGS_RELEASE "-Ofast" CACHE STRING "Release build flags" FORCE)
target_compile_options(GameEngine PRIVATE -Wall -Wextra -Wno-unknown-pragmas -fPIE)

set_target_properties(GameEngine PROPERTIES
	CXX_STANDARD 17
	CXX_STANDARD_REQUIRED YES
	CXX_EXTENSIONS NO
)
