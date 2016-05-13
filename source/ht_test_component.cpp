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

#include <ht_test_component.h>
#include <ht_debug.h>
#include <ht_scene.h>
#include <ht_python.h>

namespace Hatchit {
    namespace Game {
        Core::JSON TestComponent::VSerialize(void)
        {
            return nlohmann::json::object_t();
        }
        bool TestComponent::VDeserialize(const Core::JSON& jsonObject)
        {
            /* Fix SwigMethods to carry the callback ptrs when needed */
            //SWIG_init();
            return true;
        }
        void TestComponent::VOnInit()
        {

            
            HT_DEBUG_PRINTF("Initialized Test Component.\n");
        }

        void TestComponent::VOnUpdate()
        {
            Math::Vector3 position = m_owner->GetTransform().GetPosition();
            Py_Initialize();
            auto module = PyImport_ImportModule("TestComponent");
            if (module == nullptr)
            {
                PyErr_Print();
            }
            auto func = PyObject_GetAttrString(module, "Update");
            PyObject* result;
            if (func && PyCallable_Check(func))
            {
                PyObject* args = PyTuple_New(1);
                auto type = SWIG_TypeQuery("Hatchit::Math::Vector3 *");
                PyObject* pyvec1 = SWIG_NewPointerObj(&position, type, SWIG_POINTER_OWN);
                PyTuple_SetItem(args, 0, pyvec1);
                result = PyObject_CallObject(func, args);
                if (!result)
                {
                    HT_DEBUG_PRINTF("Python script error.\n");
                    PyErr_Print();
                }
            }
            Transform* t = &m_owner->GetTransform();
            t->SetPosition(position);
            HT_DEBUG_PRINTF("Updated Test Component.\n");
        }

        Component* TestComponent::VClone(void) const
        {
            HT_DEBUG_PRINTF("Cloned Test Component.\n");
            return new TestComponent(*this);
        }

        void TestComponent::VOnEnabled()
        {
            HT_DEBUG_PRINTF("Enabled Test Component.\n");
        }

        void TestComponent::VOnDisabled()
        {
            HT_DEBUG_PRINTF("Disabled Test Component.\n");
        }

        void TestComponent::VOnDestroy()
        {
            Py_Finalize();
            HT_DEBUG_PRINTF("Destroyed Test Component.\n");
        }
    }
}
