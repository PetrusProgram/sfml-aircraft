#pragma once
#include "SceneNode.hpp"
#include "Particle.hpp"

class ParticleNode;

class EmitterNode final : public SceneNode
{
public:
	explicit EmitterNode(Particle::Type type) : mType(type) {}
	EmitterNode(const EmitterNode& other) = delete;
	EmitterNode& operator=(const EmitterNode& other) = delete;

private:
	virtual void updateCurrent(sf::Time dt, CommandQueue& commands) override;
	void emitParticles(sf::Time dt);

private:
	sf::Time mAccumulatedTime;
	Particle::Type mType;
	ParticleNode* mParticleSystem{ nullptr };
};
