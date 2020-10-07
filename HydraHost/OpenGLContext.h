#pragma once
#include <Windows.h>

struct WindowState
{
    int MouseX;
    int MouseY;
    unsigned char MouseLeft : 1, MouseRight : 1, MouseMiddle : 1;
};

class OpenGLContext
{
    class OpenGLContextImpl *_impl = nullptr;

public:
    OpenGLContext();
    ~OpenGLContext();
    bool Initialize(HWND hwnd);
    void Render(const WindowState &windowState);
};
