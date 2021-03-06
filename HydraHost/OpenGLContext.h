#pragma once
#include <Windows.h>

class OpenGLContext
{
    class OpenGLContextImpl *_impl = nullptr;

public:
    OpenGLContext();
    ~OpenGLContext();
    bool Initialize(HWND hwnd);
    void Begin(int width, int height);
    void End();
};
