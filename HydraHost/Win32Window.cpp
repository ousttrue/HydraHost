#include "Win32Window.h"
#include <Windows.h>
#include <Windowsx.h>
#include <assert.h>

///
/// Win32WindowImpl
///
class Win32WindowImpl
{
public:
    HWND _hwnd = NULL;
    int _width = -1;
    int _height = -1;
    int _xPos = -1;
    int _yPos = -1;
    int _wheel = 0;
    bool _mouseLeft = false;
    bool _mouseRight = false;
    bool _mouseMiddle = false;
    int _retCode = 0;

    static LRESULT CALLBACK WindowProc(
        _In_ HWND hwnd,
        _In_ UINT uMsg,
        _In_ WPARAM wParam,
        _In_ LPARAM lParam)
    {
        auto self = (Win32WindowImpl *)GetWindowLongPtrA(hwnd, GWLP_USERDATA);
        if (self)
        {
            assert(self->_hwnd == hwnd);
            return self->WindowMethod(uMsg, wParam, lParam);
        }
        else
        {
            if (uMsg == WM_CREATE)
            {
                auto pcs = (LPCREATESTRUCTA)lParam;
                auto self = (Win32WindowImpl *)pcs->lpCreateParams;
                SetWindowLongPtrA(hwnd, GWLP_USERDATA, (LONG_PTR)self);
                self->_hwnd = hwnd;
                return 0;
            }
            else
            {
                return DefWindowProc(hwnd, uMsg, wParam, lParam);
            }
        }
    }

private:
    LRESULT
    WindowMethod(
        UINT uMsg,
        WPARAM wParam,
        LPARAM lParam)
    {
        switch (uMsg)
        {
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;

        case WM_SIZE:
            _width = LOWORD(lParam);
            _height = HIWORD(lParam);
            return 0;

        case WM_LBUTTONDOWN:
            _mouseLeft = true;
            SetCapture(_hwnd);
            return 0;
        case WM_LBUTTONUP:
            _mouseLeft = false;
            ReleaseCapture();
            return 0;

        case WM_RBUTTONDOWN:
            _mouseRight = true;
            SetCapture(_hwnd);
            return 0;
        case WM_RBUTTONUP:
            _mouseRight = false;
            ReleaseCapture();
            return 0;

        case WM_MBUTTONDOWN:
            _mouseMiddle = true;
            SetCapture(_hwnd);
            return 0;
        case WM_MBUTTONUP:
            _mouseMiddle = false;
            ReleaseCapture();
            return 0;

        case WM_MOUSEMOVE:
            _xPos = GET_X_LPARAM(lParam);
            _yPos = GET_Y_LPARAM(lParam);
            return 0;

        case WM_MOUSEWHEEL:
            _wheel = GET_WHEEL_DELTA_WPARAM(wParam);
            return 0;
        }

        return DefWindowProc(_hwnd, uMsg, wParam, lParam);
    }
};

///
/// Win32Window
///
Win32Window::Win32Window(Win32WindowImpl *impl)
    : _impl(impl)
{
}

Win32Window::~Win32Window()
{
    delete _impl;
}

HWND Win32Window::GetHandle() const
{
    return _impl->_hwnd;
}

std::shared_ptr<Win32Window> Win32Window::Create(std::string_view title, int width, int height)
{
    auto hInstance = GetModuleHandle(NULL);

    // platform initialize
    WNDCLASSEXA wc = {0};
    wc.cbSize = sizeof(WNDCLASSEXA);
    wc.lpfnWndProc = &Win32WindowImpl::WindowProc;
    wc.style = 0;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.lpszMenuName = NULL;
    auto CLASS_NAME = "HYDRA_HOST";
    wc.lpszClassName = CLASS_NAME;
    if (RegisterClassExA(&wc) == 0)
    {
        return nullptr;
    }

    auto impl = new Win32WindowImpl();

    auto hwnd = CreateWindowExA(0, CLASS_NAME, title.data(), WS_OVERLAPPEDWINDOW,
                                CW_USEDEFAULT, CW_USEDEFAULT,
                                width, height,
                                NULL, NULL, hInstance, impl);
    if (!hwnd)
    {
        return nullptr;
    }

    ShowWindow(hwnd, SW_SHOW);

    return std::shared_ptr<Win32Window>(new Win32Window(impl));
}

bool Win32Window::ProcessEvent(WindowState *state, int *exitCode)
{
    MSG msg;
    while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
    {
        if (msg.message == WM_QUIT)
        {
            if (exitCode)
                *exitCode = (int)msg.wParam;
            return false;
        }
        DispatchMessage(&msg);
    }

    if (state)
    {
        state->Width = _impl->_width;
        state->Height = _impl->_height;
        state->MouseX = _impl->_xPos;
        state->MouseY = _impl->_yPos;
        state->MouseLeft = _impl->_mouseLeft;
        state->MouseRight = _impl->_mouseRight;
        state->MouseMiddle = _impl->_mouseMiddle;
        state->Wheel = _impl->_wheel;
        // consume wheel
        _impl->_wheel = 0;
    }

    return true;
}
