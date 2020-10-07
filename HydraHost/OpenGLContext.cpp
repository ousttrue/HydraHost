#include "OpenGLContext.h"
#include <gl/glew.h>
#include <GL/GL.h>
#include "GL/wglext.h"
#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_opengl3.h>

// GL 3.0 + GLSL 130
const char *glsl_version = "#version 130";

///
/// OpenGLContext
///
class OpenGLContextImpl
{
    HWND _hWND = NULL;
    HDC _hDC = NULL;
    HGLRC _hGLRC = NULL;

public:
    ~OpenGLContextImpl()
    {
        wglMakeCurrent(NULL, NULL);
        wglDeleteContext(_hGLRC);
        ReleaseDC(_hWND, _hDC);
    }

    bool Initialize(HWND hwnd)
    {
        _hWND = hwnd;
        _hDC = GetDC(hwnd);
        PIXELFORMATDESCRIPTOR pfd = {0};
        pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
        pfd.nVersion = 1;
        pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
        pfd.iPixelType = PFD_TYPE_RGBA;
        pfd.cRedBits = 8;
        pfd.cGreenBits = 8;
        pfd.cBlueBits = 8;
        pfd.cAlphaBits = 8;
        pfd.cColorBits = 24;
        pfd.cDepthBits = 24;
        pfd.cStencilBits = 8;
        int pixelformat = ChoosePixelFormat(_hDC, &pfd);
        if (SetPixelFormat(_hDC, pixelformat, &pfd) == 0)
        {
            false;
        }

        _hGLRC = wglCreateContext(_hDC);
        if (!_hGLRC)
        {
            false;
        }
        wglMakeCurrent(_hDC, _hGLRC);

        //
        // upgrade WGLContext
        //
        auto wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");
        if (!wglCreateContextAttribsARB)
        {
            return false;
        }
        const int att[] = {
            WGL_CONTEXT_MAJOR_VERSION_ARB,
            4,
            WGL_CONTEXT_MINOR_VERSION_ARB,
            6,
            WGL_CONTEXT_FLAGS_ARB,
            0,
            WGL_CONTEXT_PROFILE_MASK_ARB,
            WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
            0,
        };
        auto hglrc = wglCreateContextAttribsARB(_hDC, NULL, att);
        wglMakeCurrent(_hDC, hglrc);
        wglDeleteContext(_hGLRC);
        _hGLRC = hglrc;

        return true;
    }

    void Begin()
    {
        wglMakeCurrent(_hDC, _hGLRC);
    }

    void End()
    {
        glFinish();
        SwapBuffers(_hDC);
    }
};

///
/// OpenGLContext
///
OpenGLContext::OpenGLContext()
    : _impl(new OpenGLContextImpl)
{
}

OpenGLContext::~OpenGLContext()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    delete _impl;
}

bool OpenGLContext::Initialize(HWND hwnd)
{
    if (!_impl->Initialize(hwnd))
    {
        return false;
    }

    auto err = glewInit();
    if (err != GLEW_OK)
    {
        return false;
    }

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // Setup Platform/Renderer bindings
    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplOpenGL3_Init(glsl_version);

    return true;
}

void OpenGLContext::Render()
{
    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
    static bool show_demo_window = true;
    ImGui::ShowDemoWindow(&show_demo_window);

    // Rendering
    ImGui::Render();
    ImGuiIO &io = ImGui::GetIO();

    _impl->Begin();
    {
        glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
        glClearColor(0.2f, 0.2f, 0.6f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }
    _impl->End();
}
