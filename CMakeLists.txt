cmake_minimum_required(VERSION 3.14 FATAL_ERROR)
set(PROJECT_NAME ProtocolHandler)
project(${PROJECT_NAME})
set(BUILD_SHARED_LIBS OFF)
set(CMAKE_CXX_STANDART 17)
file(GLOB sources "${PROJECT_SOURCE_DIR}/src/*.cpp"
				  "${PROJECT_SOURCE_DIR}/include/ProtocolHandler/*.h"
				  "${PROJECT_SOURCE_DIR}/src/ProtocolHandler/*.cpp"
)

add_executable(${PROJECT_NAME}
			   ${sources}
)

target_include_directories(${PROJECT_NAME} PRIVATE "${PROJECT_SOURCE_DIR}/include")