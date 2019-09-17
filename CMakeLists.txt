cmake_minimum_required(VERSION 3.6)
project(memory_hugger)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release
        CACHE STRING "Choose the type of build: Debug Release Memcheck" FORCE)
ENDIF (NOT CMAKE_BUILD_TYPE)
message("* Current build type is : ${CMAKE_BUILD_TYPE}")

# Compiler flags for various cmake build types
set(WARNINGS_FLAGS "-Wall -Wextra --pedantic -Wno-unused-parameter")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DNDEBUG -O3")
# -Wno-register because openio is using it although it shouldn't
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${WARNINGS_FLAGS} -Wno-register -g -O0")
set(CMAKE_CXX_FLAGS_MEMCHECK "${WARNINGS_FLAGS} -g -O0 -fsanitize=address -fno-omit-frame-pointer")

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMake" ${CMAKE_MODULE_PATH})

set(CMAKE_EXPORT_COMPILE_COMMANDS 0)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# boost dependencies, system is required for filesystem
find_package(Boost 1.53 REQUIRED COMPONENTS program_options)

set(INCLUDE_DIR "${CMAKE_SOURCE_DIR}/include")
# define include directories that are relevant for all targets
include_directories(
    ${INCLUDE_DIR}
)

add_executable(memory_hugger "")
target_sources(memory_hugger
    PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/main.cpp
    )
target_link_libraries(memory_hugger
    Boost::program_options
    )

install(TARGETS memory_hugger
    DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
    )
