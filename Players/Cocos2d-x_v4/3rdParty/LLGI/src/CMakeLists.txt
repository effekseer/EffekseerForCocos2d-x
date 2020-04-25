cmake_minimum_required (VERSION 3.0.0)
project(LLGI)

file(GLOB files *.h *.cpp)
file(GLOB files_pc PC/*.h PC/*.cpp)

list(APPEND files ${files_pc})

if(WIN32)
  file(GLOB files_win Win/*.h Win/*.cpp)
  list(APPEND files ${files_win})
elseif(APPLE)
  file(GLOB files_mac Mac/*.h Mac/*.cpp Mac/*.mm)
  list(APPEND files ${files_mac})
else()
  file(GLOB files_linux Linux/*.h Linux/*.cpp)
  list(APPEND files ${files_linux})
endif()

if(MSVC)
  file(GLOB files_dx12 DX12/*.h DX12/*.cpp)
  list(APPEND files ${files_dx12})
endif()

if(BUILD_VULKAN)
  file(GLOB files_vulkan Vulkan/*.h Vulkan/*.cpp)
  list(APPEND files ${files_vulkan})
  add_definitions(-DENABLE_VULKAN)
endif()

if(APPLE)
  file(GLOB files_metal Metal/*.h Metal/*.cpp Metal/*.mm)
  list(APPEND files ${files_metal})
endif()

foreach(FILE ${files}) 
    # Get the directory of the source file
    get_filename_component(PARENT_DIR "${FILE}" DIRECTORY)

    # Remove common directory prefix to make the group
    string(REPLACE "${CMAKE_CURRENT_SOURCE_DIR}" "" GROUP "${PARENT_DIR}")

    # Make sure we are using windows slashes
    string(REPLACE "/" "\\" GROUP "${GROUP}")
    
    if("${GROUP}" STREQUAL "")
        set(GROUP "Base")
    endif()
    
    # Group into "Source Files" and "Header Files"
    if ("${FILE}" MATCHES ".*\\.cpp")
       set(GROUP "${GROUP}")
    elseif("${FILE}" MATCHES ".*\\.c")
       set(GROUP "${GROUP}")
    elseif("${FILE}" MATCHES ".*\\.cxx")
       set(GROUP "${GROUP}")
    elseif("${FILE}" MATCHES ".*\\.h")
       set(GROUP "${GROUP}")
    elseif("${FILE}" MATCHES ".*\\.mm")
       set(GROUP "${GROUP}")
    endif()

    source_group("${GROUP}" FILES "${FILE}")
endforeach()

add_library(LLGI STATIC ${files})

file(GLOB LOCAL_HEADERS *.h)
set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER "${LOCAL_HEADERS}")

if(BUILD_VULKAN)
  find_package(Vulkan REQUIRED)
  target_include_directories(LLGI PRIVATE ${Vulkan_INCLUDE_DIRS})
  target_link_libraries(LLGI PRIVATE ${Vulkan_LIBRARIES})
  if(BUILD_VULKAN_COMPILER)
    target_link_libraries(LLGI PRIVATE glslang SPIRV)
  endif()
endif()

if(WIN32)
elseif(APPLE)
else()
  set_property(TARGET LLGI PROPERTY POSITION_INDEPENDENT_CODE TRUE) 
endif()

#--------------------
# Install glslang
#   FIXME: This code is "HACK" for a cmake successful configuration.
#          glslang doesn't support EXPORT, so get an error.
#          Overriding glslang installation settings.

if(BUILD_VULKAN)

  if(BUILD_VULKAN_COMPILER)

    set_target_properties(glslang PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_INSTALL_PREFIX}/include)
    set_target_properties(SPIRV PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_INSTALL_PREFIX}/include)
    target_include_directories(glslang PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/../thirdparty/glslang>)
    target_include_directories(SPIRV PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/../thirdparty/glslang>)

    install(TARGETS glslang
            EXPORT glslang-export
            INCLUDES DESTINATION include/LLGI
            PUBLIC_HEADER DESTINATION include/LLGI
            ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})
    install(EXPORT glslang-export
            FILE glslang-config.cmake
            DESTINATION lib/cmake)
        
    install(TARGETS OGLCompiler
            EXPORT OGLCompiler-export
            ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})
    install(EXPORT OGLCompiler-export
            FILE OGLCompiler-config.cmake
            DESTINATION lib/cmake)

    install(TARGETS OSDependent
            EXPORT OSDependent-export
            ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})
    install(EXPORT OSDependent-export
            FILE OSDependent-config.cmake
            DESTINATION lib/cmake)
        
    install(TARGETS HLSL
            EXPORT HLSL-export
            ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})
    install(EXPORT HLSL-export
            FILE HLSL-config.cmake
            DESTINATION lib/cmake)

    install(TARGETS SPIRV
            EXPORT SPIRV-export
            ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})
    install(EXPORT SPIRV-export
            FILE SPIRV-config.cmake
            DESTINATION lib/cmake)
  endif()
endif()


#--------------------
# Install

install(
    TARGETS ${PROJECT_NAME}
    EXPORT ${PROJECT_NAME}-export
    INCLUDES DESTINATION include/LLGI
    PUBLIC_HEADER DESTINATION include/LLGI
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib)

install(
    EXPORT ${PROJECT_NAME}-export
    FILE ${PROJECT_NAME}-config.cmake
    DESTINATION lib/cmake
    EXPORT_LINK_INTERFACE_LIBRARIES)
