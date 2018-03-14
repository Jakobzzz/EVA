#pragma once

namespace eva
{
	class Component
	{
	public:
		Component() = default;
		~Component() = default;

	public:
		unsigned int GetID() const { return id; }

	protected:
		unsigned int id;
	};
}
