cmake_minimum_required(VERSION 3.15)

project(cpp-dev-server VERSION 1.0
        DESCRIPTION "Project that helps you to speed up developing. This project launches development server that takes care about compilation and you don't need to worry about it."
        LANGUAGES CXX)

include(CheckIPOSupported)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(SOURCE_PATH "source" CACHE STRING "Path to sources")

file(TO_CMAKE_PATH "${PROJECT_BINARY_DIR}/CMakeLists.txt" LOC_PATH)
if(EXISTS "${LOC_PATH}")
    message(FATAL_ERROR "You cannot build in a source directory (or any directory with a CMakeLists.txt file). Please make a build subdirectory. Feel free to remove CMakeCache.txt and CMakeFiles.")
endif()

if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
        add_compile_options(/Ot /Zc:auto)
endif()	

if (UNIX)
        add_compile_options(-Ofast)
endif()

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
        message(STATUS "Setting build type to '${DEFAULT_BUILD_TYPE}' as none was specified.")
        set(CMAKE_BUILD_TYPE "${DEFAULT_BUILD_TYPE}" CACHE
        STRING "Choose the type of build." FORCE)
        set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
        "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

add_executable(devserver "$CACHE{SOURCE_PATH}/main.cpp")

check_ipo_supported(RESULT result)
if(result)
        set_target_properties(devserver PROPERTIES INTERPROCEDURAL_OPTIMIZATION TRUE)
endif()

add_library(dev "$CACHE{SOURCE_PATH}/dev.cpp")

if (CMAKE_SYSTEM_NAME STREQUAL "Darwin")
        add_library(overdarwin "$CACHE{API_PATH}/sys/os/darwin.cpp")
        target_link_libraries(devserver PUBLIC overdarwin)
	set(CMAKE_EXE_LINKER_FLAGS "-framework CoreFoundation -framework WebKit -framework CoreServices -framework CoreGraphics")  
endif()

target_link_libraries(devserver PUBLIC dev)

target_compile_features(devserver PUBLIC cxx_std_17)
set_target_properties(devserver PROPERTIES CXX_EXTENSIONS OFF)
target_include_directories(devserver PUBLIC ${SOURCE_PATH})

if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
        target_link_libraries(devserver PUBLIC -lpthread)
endif()
