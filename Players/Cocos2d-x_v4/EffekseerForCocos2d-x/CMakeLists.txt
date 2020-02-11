
project(EffekseerForCocos2d-x)

if (APPLE)

add_library(EffekseerForCocos2d-x
    STATIC
    EffekseerForCocos2d-x.cpp
    EffekseerForCocos2d-x.h
    Metal/Effekseer+Metal.mm
    GL/Effekseer+GL.cpp)

else()

add_library(EffekseerForCocos2d-x
    STATIC
    EffekseerForCocos2d-x.cpp
    EffekseerForCocos2d-x.h
    GL/Effekseer+GL.cpp)

endif()

target_include_directories(${PROJECT_NAME} 
    PUBLIC ../Effekseer/
    PUBLIC ../EffekseerRendererCommon/
    PUBLIC ../
    PUBLIC ./
    PRIVATE ../3rdParty/LLGI/src/)

target_link_libraries(${PROJECT_NAME} 
    PRIVATE cocos2d
    PUBLIC Effekseer)

add_dependencies(${PROJECT_NAME} Effekseer)

if (APPLE)
    target_include_directories(${PROJECT_NAME} 
        PUBLIC ../EffekseerRendererMetal/)

    target_link_libraries(${PROJECT_NAME} 
        PUBLIC EffekseerRendererMetal
        PUBLIC EffekseerRendererGL)

    add_dependencies(${PROJECT_NAME} EffekseerRendererMetal)
    add_dependencies(${PROJECT_NAME} EffekseerRendererGL)
else()
    target_include_directories(${PROJECT_NAME} 
        PUBLIC ../EffekseerRendererGL/)

    target_link_libraries(${PROJECT_NAME} 
        PUBLIC EffekseerRendererGL)

    add_dependencies(${PROJECT_NAME} EffekseerRendererGL)
endif()
