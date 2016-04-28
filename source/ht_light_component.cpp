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

#ifdef VK_SUPPORT
#include <ht_vkrenderer.h>
#include <ht_vkmaterial.h>
#include <ht_vkmesh.h>
#endif

#ifdef HT_SYS_WINDOWS
#ifdef DX12_SUPPORT
#include <ht_d3d12material.h>
#endif
#endif

#include <ht_gameobject.h>
#include <ht_light_component.h>
#include <ht_renderer_singleton.h>
#include <ht_debug.h>

namespace Hatchit {

    namespace Game {

        LightComponent::LightComponent()
        {

        }

        /**
        * \brief Saves light as JSON.
        */
        Core::JSON LightComponent::VSerialize(void)
        {
            return Core::JSON();
        }

        /**
        * \brief Constructs light from JSON.
        */
        bool LightComponent::VDeserialize(const Core::JSON& jsonObject)
        {
            int lightType;
            if (Core::JsonExtract<int32_t>(jsonObject, "LightType", lightType)) 
            {
                if (lightType == LightType::POINT_LIGHT)
                {
                    std::vector<float> attenuation;
                    std::vector<float> color;
                    if (!Core::JsonExtract<float>(jsonObject, "Radius", m_radius) 
                        || !Core::JsonExtractContainer(jsonObject, "Attenuation", attenuation)
                        || !Core::JsonExtractContainer(jsonObject, "Color", color))
                        return false;
                    m_attenuation = { attenuation[0], attenuation[1], attenuation[2] };
                    m_color = { color[0], color[1], color[2], color[3] };
                }
                m_lightType = LightType(lightType);
            }
            return true;
        }

        /**
        * \brief Sets the light to the specified type with proper data.
        * \param lightType enum value of light
        */
        void LightComponent::SetType(LightType lightType)
        {
            m_lightType = lightType;
            switch (lightType)
            {
                case LightType::POINT_LIGHT:
                {
                    SetMeshAndMaterial("Icosphere.dae", "PointLightMaterial.json");
                    break;
                }
            }
            m_meshRenderer->SetMesh(m_mesh);
            m_meshRenderer->SetMaterial(m_material);
        }

        /**
        * \brief Called when the GameObject is created to initialize all values
        */
        void LightComponent::VOnInit()
        {
            m_meshRenderer = new Graphics::MeshRenderer();

            SetType(m_lightType);
            
            HT_DEBUG_PRINTF("Initialized Light Component.\n");
        }

        /**
        * \brief Called once per frame while the GameObject is enabled.
        * Updates all components first, then all child gameobjects.
        */
        void LightComponent::VOnUpdate()
        {
            std::vector<Resource::ShaderVariable*> data;
            data.push_back(new Resource::Matrix4Variable(Math::MMMatrixTranspose(*GetOwner()->GetTransform().GetWorldMatrix())));
            data.push_back(new Resource::Float4Variable(m_color));
            data.push_back(new Resource::FloatVariable(m_radius));
            data.push_back(new Resource::Float3Variable(m_attenuation));

            m_meshRenderer->SetInstanceData(data);
            m_meshRenderer->Render();
        }

        /**
        * \brief Creates a copy of this Component.
        */
        Component* LightComponent::VClone(void) const
        {
            HT_DEBUG_PRINTF("Cloned LightComponent.\n");
            return new LightComponent(*this);
        }

        /**
        * \brief Called when the Component is enabled.
        * This happens when a scene has finished loading, or immediately after creation if the scene is already loaded.
        */
        void LightComponent::VOnEnabled()
        {
            HT_DEBUG_PRINTF("Enabled LightComponent Component.\n");
        }

        /**
        * \brief Called when the Component is disabled.
        * Components are always disabled before destroyed.
        * When a scene is destroyed, all Components are disabled before any are destroyed.
        */
        void LightComponent::VOnDisabled()
        {
            HT_DEBUG_PRINTF("Disabled LightComponent Component.\n");
        }

        /**
        * \brief Called when the GameObject is destroyed/deleted.
        * Objects are always disabled before destroyed.
        * When a scene is destroyed, all gameobjects are disabled before any are destroyed.
        */
        void LightComponent::VOnDestroy()
        {
            delete m_meshRenderer;
            HT_DEBUG_PRINTF("Destroyed LightComponent Component.\n");
        }

        /**
        * \brief Sets the light mesh with the appropriate type for current renderer.
        * \param meshFile File name of mesh to load
        * \param meshFile File name of material to load
        */
        bool LightComponent::SetMeshAndMaterial(std::string meshFile, std::string materialFile)
        {
            if (Renderer::GetRendererType() == Graphics::DIRECTX11)
                return false;
            else if (Renderer::GetRendererType() == Graphics::DIRECTX12)
                //mat = Graphics::DX::D3D12Material::GetHandle(material);
                return false;
            else if (Renderer::GetRendererType() == Graphics::VULKAN)
            {
                Resource::ModelHandle model = Resource::Model::GetHandleFromFileName(meshFile);
                std::vector<Resource::Mesh*> meshes = model->GetMeshes();

                Graphics::Vulkan::VKRenderer* renderer = dynamic_cast<Graphics::Vulkan::VKRenderer*>(Renderer::instance().GetRenderer());
                m_mesh = Graphics::Vulkan::VKMesh::GetHandle(meshFile, meshes[0], renderer).StaticCastHandle<Graphics::IMesh>();
                m_material = Graphics::Vulkan::VKMaterial::GetHandle(materialFile, materialFile, renderer).StaticCastHandle<Graphics::IMaterial>();
            }
            else if (Renderer::GetRendererType() == Graphics::OPENGL)
                return false;
            return true;
        }
    }
}
