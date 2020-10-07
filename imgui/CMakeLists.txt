set(TARGET_NAME imgui)
add_library(${TARGET_NAME} STATIC
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/imgui_demo.cpp
    ${IMGUI_DIR}/examples/imgui_impl_win32.cpp
    ${IMGUI_DIR}/examples/imgui_impl_opengl3.cpp
)
target_include_directories(${TARGET_NAME}
PUBLIC
    ${IMGUI_DIR}
    ${IMGUI_DIR}/examples
    $<$<CONFIG:DEBUG>:${BUILD_DIR}/debug/include>
    $<$<CONFIG:RELEASE>:${BUILD_DIR}/release/include>
)
target_link_libraries(${TARGET_NAME}
PUBLIC
    OpenGL32
    $<$<CONFIG:DEBUG>:${BUILD_DIR}/debug/lib/glew32.lib>
    $<$<CONFIG:RELEASE>:${BUILD_DIR}/release/lib/glew32.lib>
)
