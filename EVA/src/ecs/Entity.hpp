#pragma once
#include <vector>

namespace eva
{
	class Component;

	class Entity
	{
	public:
		Entity() = default;
		~Entity() = default;

	public:
		template<typename C>
		void AddComponent(C* component);

	public:
		//Component * GetComponent(Component* component) const;

	private:
		std::vector<Component*> m_components;
	};

	template<typename C>
	inline void Entity::AddComponent(C * component)
	{
	}
}

