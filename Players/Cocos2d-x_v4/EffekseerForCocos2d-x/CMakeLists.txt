
project(EffekseerForCocos2d-x)

add_library(EffekseerForCocos2d-x
    STATIC
    EffekseerForCocos2d-x.cpp
    EffekseerForCocos2d-x.h)

target_include_directories(${PROJECT_NAME} 
    PUBLIC ../Effekseer/)

target_link_libraries(${PROJECT_NAME} 
    PRIVATE cocos2d
    PUBLIC Effekseer)

add_dependencies(${PROJECT_NAME} Effekseer)

if (APPLE)
    target_include_directories(${PROJECT_NAME} 
        PUBLIC ../EffekseerRendererMetal/)

    target_link_libraries(${PROJECT_NAME} 
        PUBLIC EffekseerRendererMetal)

    add_dependencies(${PROJECT_NAME} EffekseerRendererMetal)
else()
    target_include_directories(${PROJECT_NAME} 
        PUBLIC ../EffekseerRendererGL/)

    target_link_libraries(${PROJECT_NAME} 
        PUBLIC EffekseerRendererGL)

    add_dependencies(${PROJECT_NAME} EffekseerRendererGL)
endif()
