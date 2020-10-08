#pragma
#include <Windows.h>
#include "WindowState.h"

class Gui
{
    class GuiImpl *_impl = nullptr;

public:
    Gui(HWND hWnd);
    ~Gui();
    bool ProcessInput(const WindowState &state);
    void Render();
};
