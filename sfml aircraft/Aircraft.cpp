#include "Aircraft.hpp"
#include "DataTables.hpp"
#include "Utility.hpp"
#include "CommandQueue.hpp"
#include "ResourceHolder.hpp"
#include "Pickup.hpp"
#include <cmath>

namespace
{
	const std::vector<AircraftData> Table = initializeAircraftData();
}

Aircraft::Aircraft(Type type, const TextureHolder& textures, const FontHolder& fonts)
	: Entity(Table[static_cast<int>(type)].hitpoints)
	, mType(type)
	, mSprite(textures.get(Table[static_cast<int>(type)].texture), Table[static_cast<int>(type)].textureRect)
	, mExplosion(textures.get(Textures::ID::Explosion))
{
	mExplosion.setFrameSize(sf::Vector2i(256, 256));
	mExplosion.setNumFrames(16);
	mExplosion.setDuration(sf::seconds(1));

	centerOrigin(mSprite);
	centerOrigin(mExplosion);

	mFireCommand.category = static_cast<unsigned int>(Category::Type::SceneAirLayer);
	mFireCommand.action = [this, &textures](SceneNode& node, sf::Time)
	{
		createBullets(node, textures);
	};

	mMissileCommand.category = static_cast<unsigned int>(Category::Type::SceneAirLayer);
	mMissileCommand.action = [this, &textures](SceneNode& node, sf::Time)
	{
		createProjectile(node, Projectile::Type::Missile, 0.f, 0.5f, textures);
	};
	
	mDropPickupCommand.category = static_cast<unsigned int>(Category::Type::SceneAirLayer);
	mDropPickupCommand.action = [this, &textures](SceneNode& node, sf::Time)
	{
		createPickup(node, textures);
	};

	auto healthDisplay{ std::make_unique<TextNode>(fonts, "") };
	mHealthDisplay = healthDisplay.get();
	attachChild(std::move(healthDisplay));

	if (getCategory() == static_cast<unsigned int>(Category::Type::PlayerAircraft))
	{
		auto missileDisplay{ std::make_unique<TextNode>(fonts, "") };
		missileDisplay->setPosition(0, 70);
		mMissileDisplay = missileDisplay.get();
		attachChild(std::move(missileDisplay));
	}

	updateTexts();
}

void Aircraft::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (isDestroyed() && mShowExplosion)
		target.draw(mExplosion, states);

	else
		target.draw(mSprite, states);
}

void Aircraft::updateCurrent(sf::Time dt, CommandQueue& commands)
{
	updateTexts();
	updateRollAnimation();

	if (isDestroyed())
	{
		checkPickupDrop(commands);
		mExplosion.update(dt);
		return;
	}

	checkProjectileLaunch(dt, commands);

	updateMovementPattern(dt);
	Entity::updateCurrent(dt, commands);
}

unsigned int Aircraft::getCategory() const
{
	if (isAllied())
		return static_cast<unsigned int>(Category::Type::PlayerAircraft);

	else
		return static_cast<unsigned int>(Category::Type::EnemyAircraft);
}

void Aircraft::remove()
{
	Entity::remove();
	mShowExplosion = false;
}

void Aircraft::increaseFireRate()
{
	if (mFireRateLevel < 10)
		++mFireRateLevel;
}

void Aircraft::increaseSpread()
{
	if (mSpreadLevel < 3)
		++mSpreadLevel;
}

void Aircraft::fire()
{
	if (Table[static_cast<int>(mType)].fireInterval != sf::Time::Zero)
		mIsFiring = true;
}

void Aircraft::launchMissile()
{
	if (mMissileAmmo > 0)
	{
		mIsLaunchingMissile = true;
		--mMissileAmmo;
	}
}

void Aircraft::updateMovementPattern(sf::Time dt)
{
	const auto& directions{ Table[static_cast<int>(mType)].directions };
	if (!directions.empty())
	{
		if (mTravelledDistance > directions[mDirectionIndex].distance)
		{
			mDirectionIndex = (mDirectionIndex + 1) % directions.size();
			mTravelledDistance = 0.f;
		}

		float radians{ toRadian(directions[mDirectionIndex].angle + 90.f) };
		float vx{ getMaxSpeed() * std::cos(radians) };
		float vy{ getMaxSpeed() * std::sin(radians) };

		setVelocity(vx, vy);

		mTravelledDistance += getMaxSpeed() * dt.asSeconds();
	}
}

void Aircraft::checkPickupDrop(CommandQueue& commands)
{
	if (!isAllied() && randomInt(6) == 0 && !mSpawnedPickup)
		commands.push(mDropPickupCommand);

		mSpawnedPickup = true;
}

void Aircraft::checkProjectileLaunch(sf::Time dt, CommandQueue& commands)
{
	if (!isAllied())
		fire();

	if (mIsFiring && mFireCountdown <= sf::Time::Zero)
	{
		commands.push(mFireCommand);
		mFireCountdown += Table[static_cast<int>(mType)].fireInterval / (mFireRateLevel + 1.f);
		mIsFiring = false;
	}
	else if (mFireCountdown > sf::Time::Zero)
	{
		mFireCountdown -= dt;
		mIsFiring = false;
	}

	if (mIsLaunchingMissile)
	{
		commands.push(mMissileCommand);
		mIsLaunchingMissile = false;
	}
}

void Aircraft::createBullets(SceneNode& node, const TextureHolder& textures) const
{
	auto type{ isAllied() ? Projectile::Type::AlliedBullet : Projectile::Type::EnemyBullet };

	switch (mSpreadLevel)
	{
	case 1:
		createProjectile(node, type, 0.0f, 0.5f, textures);
		break;

	case 2:
		createProjectile(node, type, -0.33f, 0.33f, textures);
		createProjectile(node, type, +0.33f, 0.33f, textures);
		break;

	case 3:
		createProjectile(node, type, -0.5f, 0.33f, textures);
		createProjectile(node, type, 0.0f, 0.5f, textures);
		createProjectile(node, type, +0.5f, 0.33f, textures);
		break;
	}
}

void Aircraft::createProjectile(SceneNode& node, Projectile::Type type, float xOffset, float yOffset, const TextureHolder& textures) const
{
	auto projectile{ std::make_unique<Projectile>(type, textures) };

	sf::Vector2f offset(xOffset * mSprite.getGlobalBounds().width, yOffset * mSprite.getGlobalBounds().height);
	sf::Vector2f velocity(0, projectile->getMaxSpeed());

	float sign{ isAllied() ? -1.f : +1.f };
	projectile->setPosition(getWorldPosition() + offset * sign);
	projectile->setVelocity(velocity * sign);
	node.attachChild(std::move(projectile));
}

void Aircraft::createPickup(SceneNode& node, const TextureHolder& textures) const
{
	auto type{ static_cast<Pickup::Type>(randomInt(static_cast<int>(Pickup::Type::TypeCount))) };

	auto pickup{ std::make_unique<Pickup>(type, textures) };
	pickup->setPosition(getWorldPosition());
	pickup->setVelocity(0.f, 1.f);
	node.attachChild(std::move(pickup));
}
float Aircraft::getMaxSpeed() const 
{ 
	return Table[static_cast<int>(mType)].speed; 
}

void Aircraft::updateTexts()
{
	if (isDestroyed())
		mHealthDisplay->setString("");

	else
	{
		mHealthDisplay->setString(std::to_string(getHitpoints()) + " HP");
		mHealthDisplay->setPosition(0.f, 50.f);
		mHealthDisplay->setRotation(-getRotation());
	}
	if (mMissileDisplay)
	{
		if (mMissileAmmo == 0 || isDestroyed())
			mMissileDisplay->setString("");

		else
			mMissileDisplay->setString("M: " + std::to_string(mMissileAmmo));
	}
}

void Aircraft::updateRollAnimation()
{
	if (Table[static_cast<int>(mType)].hasRollAnimation)
	{
		auto textureRect{ Table[static_cast<int>(mType)].textureRect };

		if (getVelocity().x < 0.f)
			textureRect.left += textureRect.width;

		else if (getVelocity().x > 0.f)
			textureRect.left += 2 * textureRect.width;

		mSprite.setTextureRect(textureRect);
	}
}
