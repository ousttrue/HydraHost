set(TARGET_NAME HydraHost)
add_executable(${TARGET_NAME}
    main.cpp
    Win32Window.cpp
    OpenGLContext.cpp
)
target_link_libraries(${TARGET_NAME}
PRIVATE
    OpenGL32
)
