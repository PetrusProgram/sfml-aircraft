#pragma once
#include "ResourceIdentifiers.hpp"
#include "sfml\Graphics.hpp"
#include <vector>
#include <functional>

class Aircraft;

struct Direction
{
	explicit Direction(float angle, float distance) : angle(angle), distance(distance) {}

	float angle{ 0.f };
	float distance{ 0.f };
};

struct AircraftData
{
	int	hitpoints{ 0 };
	float speed{ 0.f };
	Textures::ID texture;
	sf::IntRect	textureRect;
	sf::Time fireInterval;
	std::vector<Direction> directions;
	bool hasRollAnimation{ false };
};

struct ProjectileData
{
	int	damage{ 0 };
	float speed{ 0.f };
	Textures::ID texture;
	sf::IntRect	textureRect;
};

struct PickupData
{
	std::function<void(Aircraft&)> action;
	Textures::ID texture;
	sf::IntRect	textureRect;
};

struct ParticleData
{
	sf::Color color;
	sf::Time lifetime;
};


std::vector<AircraftData> initializeAircraftData();
std::vector<ProjectileData> initializeProjectileData();
std::vector<PickupData> initializePickupData();
std::vector<ParticleData> initializeParticleData();