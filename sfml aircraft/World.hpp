#pragma once
#include "ResourceHolder.hpp"
#include "ResourceIdentifiers.hpp"
#include "SceneNode.hpp"
#include "SpriteNode.hpp"
#include "Aircraft.hpp"
#include "CommandQueue.hpp"
#include "Command.hpp"
#include "BloomEffect.hpp"
#include <array>
#include <queue>

namespace sf
{
	class RenderTarget;
}

class World
{
public:
	explicit World(sf::RenderTarget& outputTarget, FontHolder& fonts);
	World(const World& other) = delete;
	World& operator=(const World& other) = delete;

	void update(sf::Time dt);
	void draw();
	CommandQueue& getCommandQueue() { return mCommandQueue; }

	bool hasAlivePlayer() const { return !mPlayerAircraft->isMarkedForRemoval(); }
	bool hasPlayerReachedEnd() const { return !mWorldBounds.contains(mPlayerAircraft->getPosition()); }


private:
	void loadTextures();
	void adaptPlayerPosition();
	void adaptPlayerVelocity();
	void handleCollisions();

	void buildScene();
	void addEnemies();
	void addEnemy(Aircraft::Type type, float relX, float relY);
	void spawnEnemies();
	void destroyEntitiesOutsideView();
	void guideMissiles();
	sf::FloatRect getViewBounds() const 
	{
		return sf::FloatRect(mWorldView.getCenter() - mWorldView.getSize() / 2.f, mWorldView.getSize());
	}
	sf::FloatRect getBattlefieldBounds() const;

private:
	enum class Layer
	{
		Background,
		LowerAir,
		UpperAir,
		LayerCount
	};

	struct SpawnPoint
	{
		explicit SpawnPoint(Aircraft::Type type, float x, float y) : type(type), x(x), y(y)
		{}

		Aircraft::Type type;
		float x{ 0.f };
		float y{ 0.f };
	};

private:
	sf::RenderTarget& mTarget;
	sf::RenderTexture mSceneTexture;
	sf::View mWorldView;
	TextureHolder mTextures;
	FontHolder& mFonts;

	SceneNode mSceneGraph;
	std::array<SceneNode*, static_cast<int>(Layer::LayerCount)>	mSceneLayers;
	CommandQueue mCommandQueue;

	sf::FloatRect mWorldBounds;
	sf::Vector2f mSpawnPosition;
	float mScrollSpeed{ -50.f };
	Aircraft* mPlayerAircraft{ nullptr };

	std::vector<SpawnPoint> mEnemySpawnPoints;
	std::vector<Aircraft*> mActiveEnemies;

	BloomEffect mBloomEffect;
};