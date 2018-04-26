#include "Projectile.hpp"
#include "Utility.hpp"
#include "ResourceHolder.hpp"
#include "DataTables.hpp"
#include "EmitterNode.hpp"

#include <cmath>
#include <cassert>

namespace
{
	const std::vector<ProjectileData> Table = initializeProjectileData();
}

Projectile::Projectile(Type type, const TextureHolder& textures) : Entity(1), mType(type),
	mSprite(textures.get(Table[static_cast<int>(type)].texture), Table[static_cast<int>(type)].textureRect)
{
	centerOrigin(mSprite);

	if (isGuided())
	{
		auto smoke{ std::make_unique<EmitterNode>(Particle::Type::Smoke) };
		smoke->setPosition(0.f, getBoundingRect().height / 2.f);
		attachChild(std::move(smoke));

		auto propellant{ std::make_unique<EmitterNode>(Particle::Type::Propellant) };
		propellant->setPosition(0.f, getBoundingRect().height / 2.f);
		attachChild(std::move(propellant));

	}
}

void Projectile::guideTowards(sf::Vector2f position)
{
	assert(isGuided());
	mTargetDirection = unitVector(position - getWorldPosition());
}

void Projectile::updateCurrent(sf::Time dt, CommandQueue& commands)
{
	if (isGuided())
	{
		const float approachRate{ 200.f };

		auto newVelocity{ unitVector(approachRate * dt.asSeconds() * mTargetDirection + getVelocity()) };
		newVelocity *= getMaxSpeed();
		float angle{ std::atan2(newVelocity.y, newVelocity.x) };

		setRotation(toDegree(angle) + 90.f);
		setVelocity(newVelocity);
	}
	Entity::updateCurrent(dt, commands);
}

unsigned int Projectile::getCategory() const
{
	if (mType == Type::EnemyBullet)
		return static_cast<unsigned int>(Category::Type::EnemyProjectile);
	else
		return static_cast<unsigned int>(Category::Type::AlliedProjectile);
}

float Projectile::getMaxSpeed() const
{
	return Table[static_cast<int>(mType)].speed;
}

int Projectile::getDamage() const
{
	return Table[static_cast<int>(mType)].damage;
}
