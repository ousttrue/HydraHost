#include "Win32Window.h"
#include "OpenGLContext.h"
#include "Gui.h"
#define _USE_MATH_DEFINES
#include <cmath>
#include <thread>
#include <chrono>
#include <iostream>
#include <pxr/usd/usd/stage.h>
#include <pxr/usd/usd/prim.h>
#include <pxr/usd/sdf/path.h>
#include <pxr/usdImaging/usdImagingGL/engine.h>

class Camera
{
public:
    int _frame = -1;
    int _x = -1;
    int _y = -1;
    bool _mouseRight = false;
    bool _mouseMiddle = false;
    float _rotate[2] = {0, 0};
    float _translate[3] = {0, 0, -10};

    float _fovDegree = 60.0f;
    float _aspectRatio = 1.0f;
    float _near = 1.0f;
    float _far = 100000.0f;

    bool ProcessInput(const WindowState &state)
    {
        _aspectRatio = state.AspectRatio();

        if (_frame >= 0)
        {
            float dx = state.MouseX - _x;
            float dy = state.MouseY - _y;
            if (_mouseRight)
            {
                const float f = 0.5f;
                _rotate[0] += dx * f;
                _rotate[1] += dy * f;
            }
            if (_mouseMiddle)
            {
                auto t = std::tan(_fovDegree / 180 * M_PI);
                auto l = state.Diagonal();
                _translate[0] -= (dx/l * _translate[2] * t);
                _translate[1] += (dy/l * _translate[2] * t);
            }
            if(state.Wheel<0)
            {
                _translate[2] *= 1.1f;
            }
            else if(state.Wheel>0)
            {
                _translate[2] *= 0.9f;
            }
        }
        ++_frame;
        _x = state.MouseX;
        _y = state.MouseY;
        _mouseRight = state.MouseRight;
        _mouseMiddle = state.MouseMiddle;
        return _mouseRight;
    }

    pxr::GfMatrix4d View() const
    {
        pxr::GfMatrix4d viewMatrix(1.0);
        viewMatrix *= pxr::GfMatrix4d().SetRotate(pxr::GfRotation(pxr::GfVec3d(0, 1, 0), _rotate[0]));
        viewMatrix *= pxr::GfMatrix4d().SetRotate(pxr::GfRotation(pxr::GfVec3d(1, 0, 0), _rotate[1]));
        viewMatrix *= pxr::GfMatrix4d().SetTranslate(pxr::GfVec3d(_translate[0], _translate[1], _translate[2]));
        return viewMatrix;
    }

    pxr::GfMatrix4d Projection() const
    {
        pxr::GfFrustum frustum;
        frustum.SetPerspective(_fovDegree, _aspectRatio, _near, _far);
        return frustum.ComputeProjectionMatrix();
    }
};

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
        Camera camera;

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
        bool captureCamera = false;
        while (window->ProcessEvent(&state, &exitCode))
        {
            // process input
            if (captureCamera || !gui.ProcessInput(state))
            {
                // GUI に 入力を処理されなかった => View で入力を処理する
                captureCamera = camera.ProcessInput(state);

                usdEngine->SetCameraState(camera.View(), camera.Projection());
            }

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

            gui.Render();
            opengl.End();

            // keep FPS
            std::this_thread::sleep_for(std::chrono::milliseconds(30));
        }
        return exitCode;
    }
}
