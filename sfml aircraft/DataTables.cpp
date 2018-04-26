#include "DataTables.hpp"
#include "Projectile.hpp"
#include "Aircraft.hpp"
#include "Pickup.hpp"
#include "Particle.hpp"




std::vector<AircraftData> initializeAircraftData()
{
	std::vector<AircraftData> data(static_cast<int>(Aircraft::Type::TypeCount));

	data[static_cast<int>(Aircraft::Type::Eagle)].hitpoints = 100;
	data[static_cast<int>(Aircraft::Type::Eagle)].speed = 200.f;
	data[static_cast<int>(Aircraft::Type::Eagle)].fireInterval = sf::seconds(1);
	data[static_cast<int>(Aircraft::Type::Eagle)].texture = Textures::ID::Entities;
	data[static_cast<int>(Aircraft::Type::Eagle)].textureRect = sf::IntRect(0, 0, 48, 64);
	data[static_cast<int>(Aircraft::Type::Eagle)].hasRollAnimation = true;

	data[static_cast<int>(Aircraft::Type::Raptor)].hitpoints = 20;
	data[static_cast<int>(Aircraft::Type::Raptor)].speed = 80.f;
	data[static_cast<int>(Aircraft::Type::Raptor)].texture = Textures::ID::Entities;
	data[static_cast<int>(Aircraft::Type::Raptor)].textureRect = sf::IntRect(144, 0, 84, 64);
	data[static_cast<int>(Aircraft::Type::Raptor)].directions.push_back(Direction(+45.f, 80.f));
	data[static_cast<int>(Aircraft::Type::Raptor)].directions.push_back(Direction(-45.f, 160.f));
	data[static_cast<int>(Aircraft::Type::Raptor)].directions.push_back(Direction(+45.f, 80.f));
	data[static_cast<int>(Aircraft::Type::Raptor)].fireInterval = sf::Time::Zero;
	data[static_cast<int>(Aircraft::Type::Raptor)].hasRollAnimation = false;

	data[static_cast<int>(Aircraft::Type::Avenger)].hitpoints = 40;
	data[static_cast<int>(Aircraft::Type::Avenger)].speed = 50.f;
	data[static_cast<int>(Aircraft::Type::Avenger)].texture = Textures::ID::Entities;
	data[static_cast<int>(Aircraft::Type::Avenger)].textureRect = sf::IntRect(228, 0, 60, 59);
	data[static_cast<int>(Aircraft::Type::Avenger)].directions.push_back(Direction(+45.f, 50.f));
	data[static_cast<int>(Aircraft::Type::Avenger)].directions.push_back(Direction(0.f, 50.f));
	data[static_cast<int>(Aircraft::Type::Avenger)].directions.push_back(Direction(-45.f, 100.f));
	data[static_cast<int>(Aircraft::Type::Avenger)].directions.push_back(Direction(0.f, 50.f));
	data[static_cast<int>(Aircraft::Type::Avenger)].directions.push_back(Direction(+45.f, 50.f));
	data[static_cast<int>(Aircraft::Type::Avenger)].fireInterval = sf::seconds(2);
	data[static_cast<int>(Aircraft::Type::Avenger)].hasRollAnimation = false;

	return data;
}

std::vector<ProjectileData> initializeProjectileData()
{
	std::vector<ProjectileData> data(static_cast<int>(Projectile::Type::TypeCount));

	data[static_cast<int>(Projectile::Type::AlliedBullet)].damage = 10;
	data[static_cast<int>(Projectile::Type::AlliedBullet)].speed = 300.f;
	data[static_cast<int>(Projectile::Type::AlliedBullet)].texture = Textures::ID::Entities;
	data[static_cast<int>(Projectile::Type::AlliedBullet)].textureRect = sf::IntRect(175, 64, 3, 14);

	data[static_cast<int>(Projectile::Type::EnemyBullet)].damage = 10;
	data[static_cast<int>(Projectile::Type::EnemyBullet)].speed = 300.f;
	data[static_cast<int>(Projectile::Type::EnemyBullet)].texture = Textures::ID::Entities;
	data[static_cast<int>(Projectile::Type::EnemyBullet)].textureRect = sf::IntRect(178, 64, 3, 14);

	data[static_cast<int>(Projectile::Type::Missile)].damage = 200;
	data[static_cast<int>(Projectile::Type::Missile)].speed = 150.f;
	data[static_cast<int>(Projectile::Type::Missile)].texture = Textures::ID::Entities;
	data[static_cast<int>(Projectile::Type::Missile)].textureRect = sf::IntRect(160, 64, 15, 32);

	return data;
}

std::vector<PickupData> initializePickupData()
{
	std::vector<PickupData> data(static_cast<int>(Pickup::Type::TypeCount));

	data[static_cast<int>(Pickup::Type::HealthRefill)].texture = Textures::ID::Entities;
	data[static_cast<int>(Pickup::Type::HealthRefill)].textureRect = sf::IntRect(0, 64, 40, 40);
	data[static_cast<int>(Pickup::Type::HealthRefill)].action = [](Aircraft& a) { a.repair(25); };

	data[static_cast<int>(Pickup::Type::MissileRefill)].texture = Textures::ID::Entities;
	data[static_cast<int>(Pickup::Type::MissileRefill)].textureRect = sf::IntRect(40, 64, 40, 40);
	data[static_cast<int>(Pickup::Type::MissileRefill)].action = [](Aircraft& a) { a.collectMissiles(3); };

	data[static_cast<int>(Pickup::Type::FireSpread)].texture = Textures::ID::Entities;
	data[static_cast<int>(Pickup::Type::FireSpread)].textureRect = sf::IntRect(80, 64, 40, 40);
	data[static_cast<int>(Pickup::Type::FireSpread)].action = [](Aircraft& a) { a.increaseSpread(); };

	data[static_cast<int>(Pickup::Type::FireRate)].texture = Textures::ID::Entities;
	data[static_cast<int>(Pickup::Type::FireRate)].textureRect = sf::IntRect(120, 64, 40, 40);
	data[static_cast<int>(Pickup::Type::FireRate)].action = [](Aircraft& a) { a.increaseFireRate(); };

	return data;
}

std::vector<ParticleData> initializeParticleData()
{
	std::vector<ParticleData> data(static_cast<int>(Particle::Type::ParticleCount));

	data[static_cast<int>(Particle::Type::Propellant)].color = sf::Color(255, 255, 50);
	data[static_cast<int>(Particle::Type::Propellant)].lifetime = sf::seconds(0.6f);

	data[static_cast<int>(Particle::Type::Smoke)].color = sf::Color(50, 50, 50);
	data[static_cast<int>(Particle::Type::Smoke)].lifetime = sf::seconds(4.f);

	return data;
}
