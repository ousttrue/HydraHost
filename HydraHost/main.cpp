#include "Win32Window.h"
#include "OpenGLContext.h"
#include "Gui.h"
#include <thread>
#include <chrono>

int main(int argc, char **argv)
{
    // create window
    auto window = Win32Window::Create("HydraHost", 1024, 768);
    if (!window)
    {
        return 1;
    }
    auto hwnd = window->GetHandle();

    {
        // initialize opengl
        OpenGLContext opengl;
        if (!opengl.Initialize(hwnd))
        {
            return 2;
        }

        Gui gui(hwnd);

        // main loop
        WindowState state;
        int exitCode;
        while (window->ProcessEvent(&state, &exitCode))
        {
            // rendering
            opengl.Begin(state.Width, state.Height);
            gui.Render(state);
            opengl.End();

            // keep FPS
            std::this_thread::sleep_for(std::chrono::milliseconds(30));
        }
        return exitCode;
    }
}
