#include "Entity.hpp"
#include <cassert>


void Entity::setVelocity(float vx, float vy)
{
	mVelocity.x = vx;
	mVelocity.y = vy;
}

void Entity::accelerate(float vx, float vy)
{
	mVelocity.x += vx;
	mVelocity.y += vy;
}

void Entity::repair(int points)
{
	assert(points > 0);

	mHitpoints += points;
}

void Entity::damage(int points)
{
	assert(points > 0);

	mHitpoints -= points;
}