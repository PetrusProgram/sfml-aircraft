#pragma once
#include "Category.hpp"
#include "sfml\include\SFML\Graphics.hpp"
#include <functional>

class SceneNode;
struct Command
{
	std::function<void(SceneNode&, sf::Time)> action;
	unsigned int category{ static_cast<unsigned>(Category::Type::None) };

};

template < typename GameObject, typename Function,
	typename = std::enable_if_t<std::is_base_of_v<SceneNode, GameObject>> >
std::function<void(SceneNode&, sf::Time)> derivedAction(Function&& fn)
{
	return [fn](SceneNode& node, sf::Time dt)
	{
		fn(static_cast<GameObject&>(node), dt);
	};
}