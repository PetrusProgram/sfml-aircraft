#pragma once
#include "Entity.hpp"
#include "Command.hpp"
#include "ResourceIdentifiers.hpp"
#include "Projectile.hpp"
#include "TextNode.hpp"
#include "Animation.hpp"
#include <iostream>
class Aircraft final : public Entity
{
public:
	enum class Type
	{
		Eagle,
		Raptor,
		Avenger,
		TypeCount
	};


public:
	explicit Aircraft(Type type, const TextureHolder& textures, const FontHolder& fonts);
	
	virtual unsigned int getCategory() const override;
	virtual sf::FloatRect getBoundingRect() const override
	{	
		return getWorldTransform().transformRect(mSprite.getGlobalBounds());
	}

	virtual void remove() override;
	virtual bool isMarkedForRemoval() const override
	{
		return isDestroyed() && (mExplosion.isFinished() || !mShowExplosion);
	}

	bool isAllied() const { return mType == Type::Eagle; }

	float getMaxSpeed() const;

	void increaseFireRate();
	void increaseSpread();
	void collectMissiles(unsigned int count) { mMissileAmmo += count; }

	void fire();
	void launchMissile();


private:
	virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;
	virtual void updateCurrent(sf::Time dt, CommandQueue& commands);
	void updateMovementPattern(sf::Time dt);
	void checkPickupDrop(CommandQueue& commands);
	void checkProjectileLaunch(sf::Time dt, CommandQueue& commands);

	void createBullets(SceneNode& node, const TextureHolder& textures) const;
	void createProjectile(SceneNode& node, Projectile::Type type, float xOffset, float yOffset, const TextureHolder& textures) const;
	void createPickup(SceneNode& node, const TextureHolder& textures) const;

	void updateTexts();
	void updateRollAnimation();


private:
	Type mType;
	sf::Sprite mSprite;
	Animation mExplosion;
	Command mFireCommand;
	Command	mMissileCommand;
	sf::Time mFireCountdown;
	bool mIsFiring{ false };
	bool mIsLaunchingMissile{ false };
	bool mShowExplosion{ true };
	bool mSpawnedPickup{ false };

	int	mFireRateLevel{ 1 };
	int	mSpreadLevel{ 1 };
	int	mMissileAmmo{ 3 };

	Command mDropPickupCommand;
	float mTravelledDistance{ 0.f };
	std::size_t	mDirectionIndex{ 0 };
	TextNode* mHealthDisplay{ nullptr };
	TextNode* mMissileDisplay{ nullptr };
};
