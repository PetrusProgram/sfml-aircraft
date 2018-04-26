#pragma once
#include "Entity.hpp"
#include "ResourceIdentifiers.hpp"
#include <iostream>
class Projectile final : public Entity
{
public:
	enum class Type
	{
		AlliedBullet,
		EnemyBullet,
		Missile,
		TypeCount
	};

public:
	explicit Projectile(Type type, const TextureHolder& textures);
	void guideTowards(sf::Vector2f position);
	bool isGuided() const {	return mType == Type::Missile; }

	virtual unsigned int getCategory() const override;
	virtual sf::FloatRect getBoundingRect() const override
	{
		return getWorldTransform().transformRect(mSprite.getGlobalBounds());
	}
	float getMaxSpeed() const;
	int getDamage() const;

private:
	virtual void updateCurrent(sf::Time dt, CommandQueue& commands) override;
	virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const override
	{
		target.draw(mSprite, states);
	}

private:
	Type mType;
	sf::Sprite mSprite;
	sf::Vector2f mTargetDirection;
};