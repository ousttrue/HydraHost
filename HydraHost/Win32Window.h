#pragma once
#include <memory>
#include <string_view>

class Win32Window
{
    class Win32WindowImpl *_impl = nullptr;
    Win32Window(class Win32WindowImpl *impl);

public:
    ~Win32Window();
    static std::shared_ptr<Win32Window> Create(std::string_view title, int width, int height);
    int MainLoop();
};
