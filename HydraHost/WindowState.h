#pragma once

struct WindowState
{
    int Width;
    int Height;
    int MouseX;
    int MouseY;
    unsigned char MouseLeft : 1, MouseRight : 1, MouseMiddle : 1;
};
