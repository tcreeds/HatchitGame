/**
**    Hatchit Engine
**    Copyright(c) 2015 Third-Degree
**
**    GNU Lesser General Public License
**    This file may be used under the terms of the GNU Lesser
**    General Public License version 3 as published by the Free
**    Software Foundation and appearing in the file LICENSE.LGPLv3 included
**    in the packaging of this file. Please review the following information
**    to ensure the GNU Lesser General Public License requirements
**    will be met: https://www.gnu.org/licenses/lgpl.html
**
**/

#include <ht_application.h>
#include <ht_debug.h>
#include <ht_window_singleton.h>
#include <ht_renderer_singleton.h>
#include <ht_time_singleton.h>

namespace Hatchit {

  namespace Game {

        using namespace Graphics;

        Application::Application(Core::INIReader* settings)
        {
            m_settings = settings;
        }

        int Application::Run()
        {
            if (!Initialize())
            {
                DeInitialize();
                return -1;
            }

            Time::Start();
            while (Window::IsRunning())
            {
                Time::Tick();

                Window::PollEvents();

                Renderer::ClearBuffer(ClearArgs::ColorDepthStencil);

                Renderer::Present();

                Window::SwapBuffers();

                Time::CalculateFPS();
            }

            DeInitialize();

            return 0;
        }

        bool Application::Initialize()
        {
            /*Initialize Window with values from settings file*/
            WindowParams wparams;
            wparams.title = m_settings->GetValue("WINDOW", "sTitle", std::string("Hatchit Engine"));
            wparams.x = m_settings->GetValue("WINDOW", "iX", -1);
            wparams.y = m_settings->GetValue("WINDOW", "iY", -1);
            wparams.width = m_settings->GetValue("WINDOW", "iWidth", 800);
            wparams.height = m_settings->GetValue("WINDOW", "iHeight", 600);
            wparams.displayFPS = m_settings->GetValue("WINDOW", "bFPS", false);
            wparams.debugWindowEvents = m_settings->GetValue("WINDOW", "bDebugWindowEvents", false);

            /*Initialize Renderer with values from settings file*/
            RendererParams rparams;
#ifdef HT_SYS_LINUX
            rparams.renderer = RendererType::OPENGL;
#else
            std::string renderer = m_settings->GetValue("RENDERER", "sRenderer", std::string("DIRECTX"));
            rparams.renderer = (renderer == "DIRECTX") ? RendererType::DIRECTX : RendererType::OPENGL;
#endif
            wparams.renderer = rparams.renderer;

            if (!Window::Initialize(wparams))
                return false;


            rparams.window = Window::NativeHandle();
            rparams.clearColor = Color( m_settings->GetValue("RENDERER", "fClearR", 0.0f),
                                        m_settings->GetValue("RENDERER", "fClearG", 0.0f),
                                        m_settings->GetValue("RENDERER", "fClearB", 0.0f),
                                        m_settings->GetValue("RENDERER", "fClearA", 0.0f));
            if (!Renderer::Initialize(rparams))
                return false;

            return true;
        }

        void Application::DeInitialize()
        {
            Renderer::DeInitialize();
            Window::DeInitialize();
        }
  }

}
