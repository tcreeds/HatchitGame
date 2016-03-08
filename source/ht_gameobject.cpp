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

#include <ht_gameobject.h>
#include <ht_debug.h>

namespace Hatchit {

    namespace Game {
        GameObject::GameObject(void)
        {
            m_enabled = false;
            m_parent = nullptr;
            m_components = std::vector<Component*>(GameObject::MAX_COMPONENTS, nullptr);
            m_children = std::vector<GameObject*>();
            m_componentMask = std::bitset<GameObject::MAX_COMPONENTS>();
        }

		GameObject::GameObject(const Guid& guid)
            : GameObject()
		{
            m_guid = guid;
		}

        GameObject::~GameObject(void)
        {
            for (std::size_t i = 0; i < GameObject::MAX_COMPONENTS; ++i)
            {
                if(m_componentMask.test(i))
                {
                    delete m_components[i];
                    m_components[i] = nullptr;
                }
            }
        }

        Transform* GameObject::GetTransform()
        {
            return (Transform*)&m_transform;
        }

        bool GameObject::GetEnabled(void) const
        {
            return m_enabled;
        }

        void GameObject::SetEnabled(bool value)
        {
            m_enabled = value;
        }

        GameObject* GameObject::GetParent(void)
        {
            return m_parent;
        }

        void GameObject::SetParent(GameObject *parent)
        {
#ifdef _DEBUG
            Core::DebugPrintF("GameObject SetParent. (not implemented)\n");
#endif
        }

        void GameObject::Update(void)
        {
            for (std::size_t i = 0; i < GameObject::MAX_COMPONENTS; ++i)
            {
                if (m_componentMask.test(i))
                {
                    Component *component = m_components[i];
                    if(component->GetEnabled())
                        component->VOnUpdate();
                }
            }

            for (std::size_t i = 0; i < m_children.size(); ++i)
            {
                GameObject *child = m_children[i];
                if (child->GetEnabled())
                    child->Update();
            }
        }

        void GameObject::OnInit(void)
        {
#ifdef _DEBUG
            Core::DebugPrintF("GameObject OnInit. (not implemented)\n");
#endif
        }

        void GameObject::OnDestroy(void)
        {
            Disable();

            for (std::size_t i = 0; i < GameObject::MAX_COMPONENTS; ++i)
            {
                if (m_componentMask.test(i))
                {
                    Component *component = m_components[i];
                    if (component->GetEnabled())
                        component->SetEnabled(false);
                    component->VOnDestroy();
                }
            }

            for (std::size_t i = 0; i < m_children.size(); ++i)
            {
                GameObject *child = m_children[i];
                if (child)
                    child->OnDestroy();
            }
        }

        GameObject* GameObject::GetChildAtIndex(std::size_t index)
        {
            if (index >= m_children.size())
                return nullptr;

            return m_children[index];
        }

        void GameObject::AddChild(GameObject *child)
        {
#ifdef _DEBUG
            Core::DebugPrintF("GameObject AddChild. (not implemented)\n");
#endif
        }

        void GameObject::RemoveChildAtIndex(std::size_t index)
        {
#ifdef _DEBUG
            Core::DebugPrintF("GameObject RemoveChildAtIndex. (not implemented)\n");
#endif
        }
    }
}
