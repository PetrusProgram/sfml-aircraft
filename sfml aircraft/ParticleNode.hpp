#pragma once
#include "SceneNode.hpp"
#include "Particle.hpp"
#include "ResourceIdentifiers.hpp"
#include <deque>


class ParticleNode final : public SceneNode
{
public:
	explicit ParticleNode(Particle::Type type, const TextureHolder& textures);
	ParticleNode() = delete;
	void addParticle(sf::Vector2f position);
	Particle::Type getParticleType() const { return mType; }
	virtual unsigned int getCategory() const override
	{
		return static_cast<unsigned int>(Category::Type::ParticleSystem);
	}


private:
	virtual void updateCurrent(sf::Time dt, CommandQueue& commands) override;
	virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;
	void addVertex(float worldX, float worldY, float texCoordX, float texCoordY, const sf::Color& color) const;
	void computeVertices() const;

private:
	std::deque<Particle> mParticles;
	const sf::Texture& mTexture;
	Particle::Type mType;

	mutable sf::VertexArray mVertexArray{ sf::Quads } ;
	mutable bool mNeedsVertexUpdate{ true };
};
