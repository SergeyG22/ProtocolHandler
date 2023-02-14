cmake_minimum_required(VERSION 3.14 FATAL_ERROR)
set(PROJECT_NAME ProtocolHandler)
project(${PROJECT_NAME})
set(BUILD_SHARED_LIBS OFF)
set(CMAKE_CXX_STANDART 17)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
enable_testing()

set(BUILD_SHARED_LIBS OFF)

file(GLOB sources "${PROJECT_SOURCE_DIR}/src/*.cpp"
				  "${PROJECT_SOURCE_DIR}/include/ProtocolHandler/*.h"
				  "${PROJECT_SOURCE_DIR}/src/ProtocolHandler/*.cpp"
)

file(GLOB tests "${PROJECT_SOURCE_DIR}/tests/*.cpp"
				"${PROJECT_SOURCE_DIR}/src/ProtocolHandler/*.cpp"
				"${PROJECT_SOURCE_DIR}/include/ProtocolHandler/*.cpp"
)

add_executable(${PROJECT_NAME}
			   ${sources}
)

add_executable(
	google_tests
	${tests}
)
target_include_directories(google_tests PRIVATE "${PROJECT_SOURCE_DIR}/include")

include(GoogleTest)
gtest_discover_tests(google_tests)

target_link_libraries(google_tests gtest_main)
target_include_directories(${PROJECT_NAME} PRIVATE "${PROJECT_SOURCE_DIR}/include")

