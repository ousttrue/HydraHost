set(TARGET_NAME usd)
add_library(${TARGET_NAME} INTERFACE)
target_include_directories(${TARGET_NAME}
INTERFACE
    $<$<CONFIG:DEBUG>:${BUILD_DIR}/debug/include>
    $<$<CONFIG:DEBUG>:${BUILD_DIR}/debug/include/boost-1_70>
    $<$<CONFIG:RELEASE>:${BUILD_DIR}/release/include>
    $<$<CONFIG:RELEASE>:${BUILD_DIR}/release/include/boost-1_70>
)
target_compile_definitions(usd INTERFACE
    NOMINMAX
)
target_compile_options(usd INTERFACE
    /wd4244
    /wd4305
)
target_link_libraries(${TARGET_NAME}
INTERFACE
    OpenGL32

    "$<$<CONFIG:Debug>:$ENV{USD_BUILD_DEBUG}/lib/tbb_debug.lib>"
    "$<$<CONFIG:RelWithDebInfo>:$ENV{USD_BUILD_RELEASE}/lib/tbb.lib>"
    "$<$<CONFIG:Release>:$ENV{USD_BUILD_RELEASE}/lib/tbb.lib>"
    "$<$<CONFIG:MinSizeRel>:$ENV{USD_BUILD_RELEASE}/lib/tbb.lib>"
    
    "$<$<CONFIG:Debug>:$ENV{USD_BUILD_DEBUG}/lib/usd_ms.lib>"
    "$<$<CONFIG:RelWithDebInfo>:$ENV{USD_BUILD_RELEASE}/lib/usd_ms.lib>"
    "$<$<CONFIG:Release>:$ENV{USD_BUILD_RELEASE}/lib/usd_ms.lib>"
    "$<$<CONFIG:MinSizeRel>:$ENV{USD_BUILD_RELEASE}/lib/usd_ms.lib>"
    
    "$<$<CONFIG:Debug>:$ENV{USD_BUILD_DEBUG}/lib/glew32.lib>"
    "$<$<CONFIG:RelWithDebInfo>:$ENV{USD_BUILD_RELEASE}/lib/glew32.lib>"
    "$<$<CONFIG:Release>:$ENV{USD_BUILD_RELEASE}/lib/glew32.lib>"
    "$<$<CONFIG:MinSizeRel>:$ENV{USD_BUILD_RELEASE}/lib/glew32.lib>"  
)
