#pragma once
#include <memory>
#include <string_view>
#include <Windows.h>
#include "WindowState.h"

class Win32Window
{
    class Win32WindowImpl *_impl = nullptr;
    Win32Window(class Win32WindowImpl *impl);

public:
    ~Win32Window();
    HWND GetHandle() const;
    static std::shared_ptr<Win32Window> Create(std::string_view title, int width, int height);
    bool ProcessEvent(WindowState *state = nullptr, int *exitCode = nullptr);
};
