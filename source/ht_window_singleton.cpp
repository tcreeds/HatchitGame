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

#include <ht_window_singleton.h>
#include <ht_debug.h>
#include <ht_sdlwindow.h>

namespace Hatchit {

    namespace Game {

        bool Window::Initialize(const WindowParams& params)
        {
            Window& _instance = Window::instance();

            _instance.m_window = new SDLWindow(params);
            if (!_instance.m_window->VInitialize())
            {
#ifdef _DEBUG
                Core::DebugPrintF("Failed to initialize Window. Exiting. \n");
#endif
                return false;
            }

            return true;
        }

        void Window::DeInitialize()
        {
            Window& _instance = Window::instance();

            delete _instance.m_window;
        }

        void Window::PollEvents()
        {
            Window& _instance = Window::instance();

            _instance.m_window->VPollEvents();
        }

        void Window::Close()
        {
            Window& _instance = Window::instance();

            _instance.m_window->VClose();
        }

        bool Window::IsRunning()
        {
            Window& _instance = Window::instance();
            
            return _instance.m_window->VIsRunning();
        }

        void Window::SwapBuffers()
        {
            Window& _instance = Window::instance();

            _instance.m_window->VSwapBuffers();
        }

        void* Window::NativeHandle()
        {
            Window& _instance = Window::instance();

            return _instance.m_window->VNativeHandle();
        }
    }

}