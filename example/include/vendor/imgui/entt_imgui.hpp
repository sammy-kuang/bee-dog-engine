// for the license, see the end of the file
#pragma once

#include <map>
#include <set>
#include <functional>
#include <string>
#include <iostream>

#include <entt/entt.hpp>
#include <imgui/imgui.h>

#ifndef MM_IEEE_ASSERT
#define MM_IEEE_ASSERT(x) assert(x)
#endif

#define MM_IEEE_IMGUI_PAYLOAD_TYPE_ENTITY "MM_IEEE_ENTITY"

#ifndef MM_IEEE_ENTITY_WIDGET
#define MM_IEEE_ENTITY_WIDGET EntityWidget
#endif


template <class Component, class EntityType>
void ComponentEditorWidget([[maybe_unused]] entt::basic_registry<EntityType>& registry, [[maybe_unused]] EntityType entity) {}

template <class Component, class EntityType>
void ComponentAddAction(entt::basic_registry<EntityType>& registry, EntityType entity)
{
	registry.template emplace<Component>(entity);
}

template <class Component, class EntityType>
void ComponentRemoveAction(entt::basic_registry<EntityType>& registry, EntityType entity)
{
	registry.template remove<Component>(entity);
}

struct ComponentInfo {
	using Callback = std::function<void(entt::registry&, entt::entity)>;
	std::string name;
	Callback widget, create, destroy;
};

template <class EntityType>
class ComponentRegistrar {
public:
	using Registry = entt::basic_registry<EntityType>;
	using ComponentTypeID = entt::id_type;


	bool show_window = true;

	std::map<ComponentTypeID, ComponentInfo> component_infos;

	bool entityHasComponent(Registry& registry, EntityType& entity, ComponentTypeID type_id)
	{
		const auto* storage_ptr = registry.storage(type_id);
		return storage_ptr != nullptr && storage_ptr->contains(entity);
	}

	template <class Component>
	ComponentInfo& registerComponent(const ComponentInfo& component_info)
	{
		auto index = entt::type_hash<Component>::value();
		auto insert_info = component_infos.insert_or_assign(index, component_info);
		MM_IEEE_ASSERT(insert_info.second);
		return std::get<ComponentInfo>(*insert_info.first);
	}

	template <class Component>
	ComponentInfo& registerComponent(const std::string& name, typename ComponentInfo::Callback widget)
	{
		return registerComponent<Component>(ComponentInfo{
			name,
			widget,
			ComponentAddAction<Component, EntityType>,
			ComponentRemoveAction<Component, EntityType>,
			});
	}

	template <class Component>
	ComponentInfo& registerComponent(const std::string& name)
	{
		return registerComponent<Component>(name, ComponentEditorWidget<Component, EntityType>);
	}

};


// MIT License

// Copyright (c) 2019-2022 Erik Scholz
// Copyright (c) 2020 Gnik Droy

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.



