#pragma once
#include "SceneNode.hpp"

class Entity : public SceneNode
{
public:
	explicit Entity(int hitpoints) : mHitpoints(hitpoints) {}
	virtual ~Entity() = default;
	void setVelocity(const sf::Vector2f& velocity) { mVelocity = velocity; }
	void setVelocity(float vx, float vy);
	void accelerate(sf::Vector2f velocity) { mVelocity += velocity; }
	void accelerate(float vx, float vy);
	sf::Vector2f getVelocity() const { return mVelocity; }

	int getHitpoints() const { return mHitpoints; }
	void repair(int points);
	void damage(int points);
	void destroy() { mHitpoints = 0; }
	virtual void remove() { destroy(); }
	virtual bool isDestroyed() const override { return mHitpoints <= 0; }

protected:
	virtual void updateCurrent(sf::Time dt, CommandQueue&) { move(mVelocity * dt.asSeconds()); }

private:
	sf::Vector2f mVelocity;
	int	mHitpoints{ 0 };
};