#include "Win32Window.h"

int main()
{
    auto window = Win32Window::Create("HydraHost", 640, 480);
    if(!window)
    {
        return 1;
    }

    return window->MainLoop();
}
