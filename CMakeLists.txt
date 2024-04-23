cmake_minimum_required(VERSION 3.0.0)
project(main VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)

set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_CXX_FLAGS "")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

message(STATUS "CXX: ${CMAKE_CXX_STANDARD}, Build type: ${CMAKE_BUILD_TYPE}, Flags: All: ${CMAKE_CXX_FLAGS}, Debug: ${CMAKE_CXX_FLAGS_DEBUG}, Release: ${CMAKE_CXX_FLAGS_RELEASE}")

include(CTest)
enable_testing()

# OpenGL
find_package(OpenGL REQUIRED)

set( SRC 
	main.cpp
	${PROJECT_SOURCE_DIR}/src/glad.c
	${PROJECT_SOURCE_DIR}/src/shader.cpp
	${PROJECT_SOURCE_DIR}/src/stb_image.cpp
)

include_directories(
	${PROJECT_SOURCE_DIR}/include
	${PROJECT_SOURCE_DIR}/src
)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

file(COPY ${PROJECT_SOURCE_DIR}/resources
     DESTINATION ${EXECUTABLE_OUTPUT_PATH})
file(COPY ${PROJECT_SOURCE_DIR}/shaders
     DESTINATION ${EXECUTABLE_OUTPUT_PATH})

add_executable(main ${SRC} )

target_link_libraries(main
	${OPENGL_LIBRARY} # filled by "find_package(OpenGL REQUIRED)"
	${PROJECT_SOURCE_DIR}/lib/libglfw3.a
)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
