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

#include <ht_script_component.h>
#include <ht_debug.h>
#include <ht_scene.h>
#include <ht_python.h>

namespace Hatchit {
    namespace Game {
        Core::JSON ScriptComponent::VSerialize(void)
        {
            return nlohmann::json::object_t();
        }
        bool ScriptComponent::VDeserialize(const Core::JSON& jsonObject)
        {
            if (!(Core::JsonExtract<std::string>(jsonObject, "ScriptName", scriptName)))
                return false;        
            return true;
        }
        void ScriptComponent::VOnInit()
        {
            Py_Initialize();
            auto module = PyImport_ImportModule(scriptName.c_str());
            if (module == nullptr)
            {
                PyErr_Print();
            }
            updateFunction = PyObject_GetAttrString(module, "Update");
            
            HT_DEBUG_PRINTF("Initialized Test Component.\n");
        }

        void ScriptComponent::VOnUpdate()
        {
            PyObject* result;
            if (updateFunction && PyCallable_Check(updateFunction))
            {
                PyObject* args = PyTuple_New(1);
                auto type = SWIG_TypeQuery("Hatchit::Game::Component *");
                PyObject* pyvec1 = SWIG_NewPointerObj(this, type, SWIG_POINTER_OWN);
                PyTuple_SetItem(args, 0, pyvec1);
                result = PyObject_CallObject(updateFunction, args);
                if (!result)
                {
                    HT_DEBUG_PRINTF("Python script error.\n");
                    PyErr_Print();
                }
            }
            HT_DEBUG_PRINTF("Updated Script Component.\n");
        }

        Component* ScriptComponent::VClone(void) const
        {
            HT_DEBUG_PRINTF("Cloned Script Component.\n");
            return new ScriptComponent(*this);
        }

        /**
        * \brief Retrieves the id associated with this class of Component.
        * \return The Core::Guid associated with this Component type.
        * \sa Component(), GameObject()
        */
        Core::Guid ScriptComponent::VGetComponentId(void) const
        {
            return Component::GetComponentId<ScriptComponent>();
        }

        void ScriptComponent::VOnEnabled()
        {
            HT_DEBUG_PRINTF("Enabled Script Component.\n");
        }

        void ScriptComponent::VOnDisabled()
        {
            HT_DEBUG_PRINTF("Disabled Script Component.\n");
        }

        void ScriptComponent::VOnDestroy()
        {
            Py_Finalize();
            HT_DEBUG_PRINTF("Destroyed Script Component.\n");
        }
    }
}