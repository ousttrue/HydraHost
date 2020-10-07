#pragma
#include <Windows.h>

struct WindowState
{
    int MouseX;
    int MouseY;
    unsigned char MouseLeft : 1, MouseRight : 1, MouseMiddle : 1;
};

class Gui
{
    class GuiImpl *_impl = nullptr;

public:
    Gui(HWND hWnd);
    ~Gui();
    void Render(const WindowState &state);
};
