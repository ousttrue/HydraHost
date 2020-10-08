#include "Win32Window.h"
#include "OpenGLContext.h"
#include "Gui.h"
#include <thread>
#include <chrono>
#include <pxr/usd/usd/stage.h>
#include <pxr/usd/usd/prim.h>
#include <pxr/usd/sdf/path.h>
#include <pxr/usdImaging/usdImagingGL/engine.h>

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        return 3;
    }

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

        // usd
        pxr::UsdStageRefPtr stage;
        std::shared_ptr<pxr::UsdImagingGLEngine> usdEngine;
        if (argc > 1)
        {
            stage = pxr::UsdStage::Open(argv[1]);
            pxr::SdfPathVector excludedPaths;
            usdEngine = std::make_shared<pxr::UsdImagingGLEngine>(stage->GetPseudoRoot().GetPath(), excludedPaths);
        }

        // main loop
        WindowState state;
        int exitCode;
        while (window->ProcessEvent(&state, &exitCode))
        {
            // rendering
            opengl.Begin(state.Width, state.Height);

            if (usdEngine)
            {
                pxr::GfVec4d viewport(0, 0, state.Width, state.Height);
                usdEngine->SetRenderViewport(viewport);

                pxr::UsdImagingGLRenderParams params;
                for (; true;)
                {
                    usdEngine->Render(stage->GetPseudoRoot(), params);
                    if (usdEngine->IsConverged())
                    {
                        break;
                    }
                }
            }

            gui.Render(state);
            opengl.End();

            // keep FPS
            std::this_thread::sleep_for(std::chrono::milliseconds(30));
        }
        return exitCode;
    }
}
