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

#include <Python.h>

namespace Hatchit {
    namespace Game {
        Core::JSON TestComponent::VSerialize(void)
        {
            return nlohmann::json::object_t();
        }
        bool TestComponent::VDeserialize(const Core::JSON& jsonObject)
        {
            return false;
        }
        void TestComponent::VOnInit()
        {
            Py_Initialize();
            auto name = PyBytes_FromString("../../VS2015/HatchitMath/HatchitMath.py");
            auto module = PyImport_Import(name);
            auto func = PyObject_GetAttrString(module, "Dot");
            if (func && PyCallable_Check(func))
            {
                PyObject* args = PyTuple_New(2);
                Math::Vector3 vec1;
                Math::Vector3 vec2;
                vec1.x = 0.5;
                vec1.y = 0.5;
                vec2.y = 1;
                PyObject* pyvec1 = PyCapsule_New(&vec1, "_p_Vector", nullptr);
                PyObject* pyvec2 = PyCapsule_New(&vec2, "_p_Vector", nullptr);
                PyTuple_SetItem(args, 0, pyvec1);
                PyTuple_SetItem(args, 1, pyvec2);
                PyObject* pyval = PyObject_CallObject(func, args);
                float val = (float)PyFloat_AsDouble(pyval);
            }

            HT_DEBUG_PRINTF("Initialized Test Component.\n");
        }

        void TestComponent::VOnUpdate()
        {
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
