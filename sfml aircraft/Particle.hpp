#pragma once
#include "sfml\Graphics.hpp"

struct Particle
{
	enum class Type
	{
		Propellant,
		Smoke,
		ParticleCount
	};

	sf::Vector2f position;
	sf::Color color;
	sf::Time lifetime;
};
