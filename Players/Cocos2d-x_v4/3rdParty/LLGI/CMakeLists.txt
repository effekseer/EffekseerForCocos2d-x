cmake_minimum_required(VERSION 3.12)

# The project's name
project(LLGI)

include(ExternalProject)
set (CMAKE_CXX_STANDARD 11)

# linux flag
if(UNIX AND NOT APPLE)
  set(LINUX TRUE)
endif()

option(BUILD_VULKAN "build vulkan" OFF)
option(BUILD_TEST "build test" OFF)
option(BUILD_EXAMPLE "build examples" OFF)

if(LINUX)
  set(BUILD_VULKAN TRUE)
endif()

option(USE_MSVC_RUNTIME_LIBRARY_DLL "compile as multithreaded DLL" ON)

# MSVC Flag
if (MSVC)
  if (NOT USE_MSVC_RUNTIME_LIBRARY_DLL)
    foreach (flag CMAKE_C_FLAGS
      CMAKE_C_FLAGS_DEBUG
      CMAKE_C_FLAGS_RELEASE
      CMAKE_CXX_FLAGS
      CMAKE_CXX_FLAGS_DEBUG
      CMAKE_CXX_FLAGS_RELEASE)

      if (${flag} MATCHES "/MD")
        string(REGEX REPLACE "/MD" "/MT" ${flag} "${${flag}}")
      endif()
      if (${flag} MATCHES "/MDd")
        string(REGEX REPLACE "/MDd" "/MTd" ${flag} "${${flag}}")
      endif()
    endforeach()
  endif()
endif()

if(BUILD_VULKAN)
  add_definitions(-DENABLE_VULKAN)
endif()

if(APPLE)
  add_definitions(-DENABLE_METAL)
endif()

add_subdirectory("src")

if(BUILD_TEST)
  ExternalProject_Add(
    ExternalProject_googletest
    SOURCE_DIR        ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/googletest
    BINARY_DIR        ${CMAKE_CURRENT_BINARY_DIR}/ThirdParty/Build/googletest
    PREFIX            ${CMAKE_CURRENT_BINARY_DIR}/ThirdParty/Build/googletest
    INSTALL_DIR       ${CMAKE_CURRENT_BINARY_DIR}/ThirdParty/Install/googletest
    CMAKE_ARGS        -DCMAKE_INSTALL_PREFIX=${CMAKE_CURRENT_BINARY_DIR}/ThirdParty/Install/googletest
                      -Dgtest_force_shared_crt=${USE_MSVC_RUNTIME_LIBRARY_DLL}
  )
  set(LLGI_GOOGLETEST_INCLUDE ${CMAKE_CURRENT_BINARY_DIR}/ThirdParty/Install/googletest/include)
  set(LLGI_GOOGLETEST_LIBRARY_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/ThirdParty/Install/googletest/lib)
  add_subdirectory("src_test")
endif()

if(BUILD_EXAMPLE)
  add_subdirectory("examples")
endif()