#pragma once
#include <cmath>

struct WindowState
{
    int Width;
    int Height;
    int MouseX;
    int MouseY;
    unsigned char MouseLeft : 1, MouseRight : 1, MouseMiddle : 1;
    int Wheel;

    float AspectRatio() const { return float(Width) / Height; }
    float Diagonal() const { return (float)std::sqrt(Width * Width + Height * Height); }
};
