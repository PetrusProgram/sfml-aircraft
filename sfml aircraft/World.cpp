#include "World.hpp"
#include "Projectile.hpp"
#include "Projectile.hpp"
#include "TextNode.hpp"
#include "ParticleNode.hpp"
#include "Pickup.hpp"
#include <algorithm>
#include <cmath>
#include <limits>

World::World(sf::RenderTarget& outputTarget, FontHolder& fonts)
	: mTarget(outputTarget), mWorldView(outputTarget.getDefaultView())
	, mFonts(fonts), mWorldBounds(0.f, 0.f, mWorldView.getSize().x, 5000.f)
	, mSpawnPosition(mWorldView.getSize().x / 2.f, mWorldBounds.height - mWorldView.getSize().y / 2.f)

{
	mSceneTexture.create(mTarget.getSize().x, mTarget.getSize().y);

	loadTextures();
	buildScene();

	mWorldView.setCenter(mSpawnPosition);
}

void World::update(sf::Time dt)
{
	mWorldView.move(0.f, mScrollSpeed * dt.asSeconds());
	mPlayerAircraft->setVelocity(0.f, 0.f);

	destroyEntitiesOutsideView();
	guideMissiles();

	while (!mCommandQueue.isEmpty())
		mSceneGraph.onCommand(mCommandQueue.pop(), dt);

	adaptPlayerVelocity();

	handleCollisions();

	mSceneGraph.removeWrecks();
	spawnEnemies();

	mSceneGraph.update(dt, mCommandQueue);
	adaptPlayerPosition();
}

void World::draw()
{
	if (PostEffect::isSupported())
	{
		mSceneTexture.clear();
		mSceneTexture.setView(mWorldView);
		mSceneTexture.draw(mSceneGraph);
		mSceneTexture.display();
		mBloomEffect.apply(mSceneTexture, mTarget);
	}
	else
	{
		mTarget.setView(mWorldView);
		mTarget.draw(mSceneGraph);
		
	}
}

void World::loadTextures()
{
	mTextures.load(Textures::ID::Explosion, "Media/Textures/Explosion.png");
	mTextures.load(Textures::ID::Particle, "Media/Textures/Particle.png");
	mTextures.load(Textures::ID::FinishLine, "Media/Textures/FinishLine.png");
	mTextures.load(Textures::ID::Entities, "Media/Textures/Entities.png");
	mTextures.load(Textures::ID::Jungle, "Media/Textures/Jungle.png");

}

void World::adaptPlayerPosition()
{
	sf::FloatRect viewBounds = getViewBounds();
	static constexpr float borderDistance = 40.f;
	sf::Vector2f position = mPlayerAircraft->getPosition();
	position.x = std::max(position.x, viewBounds.left + borderDistance);
	position.x = std::min(position.x, viewBounds.left + viewBounds.width - borderDistance);
	position.y = std::max(position.y, viewBounds.top + borderDistance);
	position.y = std::min(position.y, viewBounds.top + viewBounds.height - borderDistance);
	mPlayerAircraft->setPosition(position);
}

void World::adaptPlayerVelocity()
{
	auto velocity{ mPlayerAircraft->getVelocity() };

	if (velocity.x != 0.f && velocity.y != 0.f)
		mPlayerAircraft->setVelocity(velocity / std::sqrt(2.f));

	mPlayerAircraft->accelerate(0.f, mScrollSpeed);
}

bool matchesCategories(SceneNode::Pair& colliders, Category::Type type1, Category::Type type2)
{
	auto category1{ colliders.first->getCategory() };
	auto category2{ colliders.second->getCategory() };

	if (static_cast<unsigned int>(type1) & category1 && static_cast<unsigned int>(type2) & category2)
		return true;
	
	else if (static_cast<unsigned int>(type1) & category2 && static_cast<unsigned int>(type2) & category1)
	{
		std::swap(colliders.first, colliders.second);
		return true;
	}
	else
		return false;
}

void World::handleCollisions()
{
	std::set<SceneNode::Pair> collisionPairs;
	mSceneGraph.checkSceneCollision(mSceneGraph, collisionPairs);

	for (SceneNode::Pair pair : collisionPairs)
	{
		if (matchesCategories(pair, Category::Type::PlayerAircraft, Category::Type::EnemyAircraft))
		{
			auto& player{ static_cast<Aircraft&>(*pair.first) };
			auto& enemy{ static_cast<Aircraft&>(*pair.second) };

			player.damage(enemy.getHitpoints());
			enemy.destroy();
		}

		else if (matchesCategories(pair, Category::Type::PlayerAircraft, Category::Type::Pickup))
		{
			auto& player{ static_cast<Aircraft&>(*pair.first) };
			auto& pickup{ static_cast<Pickup&>(*pair.second) };

			pickup.apply(player);
			pickup.destroy();
		}

		else if (matchesCategories(pair, Category::Type::EnemyAircraft, Category::Type::AlliedProjectile)
			|| matchesCategories(pair, Category::Type::PlayerAircraft, Category::Type::EnemyProjectile))
		{
			auto& aircraft{ static_cast<Aircraft&>(*pair.first) };
			auto& projectile{ static_cast<Projectile&>(*pair.second) };

			aircraft.damage(projectile.getDamage());
			projectile.destroy();
		}
	}
}

void World::buildScene()
{
	for (std::size_t i{ 0 }; i < static_cast<int>(Layer::LayerCount); ++i)
	{
		auto category{ (i == static_cast<int>(Layer::LowerAir)) ? Category::Type::SceneAirLayer
			: Category::Type::None };

		auto layer{ std::make_unique<SceneNode>(category) };
		mSceneLayers[i] = layer.get();

		mSceneGraph.attachChild(std::move(layer));
	}

	auto& jungleTexture{ mTextures.get(Textures::ID::Jungle) };
	jungleTexture.setRepeated(true);

	float viewHeight{ mWorldView.getSize().y };
	sf::IntRect textureRect(mWorldBounds);
	textureRect.height += static_cast<int>(viewHeight);

	auto jungleSprite{ std::make_unique<SpriteNode>(jungleTexture, textureRect) };
	jungleSprite->setPosition(mWorldBounds.left, mWorldBounds.top - viewHeight);
	mSceneLayers[static_cast<int>(Layer::Background)]->attachChild(std::move(jungleSprite));

	auto& finishTexture{ mTextures.get(Textures::ID::FinishLine) };
	auto finishSprite{ std::make_unique<SpriteNode>(finishTexture) };
	finishSprite->setPosition(0.f, -76.f);
	mSceneLayers[static_cast<int>(Layer::Background)]->attachChild(std::move(finishSprite));

	auto smokeNode{ std::make_unique<ParticleNode>(Particle::Type::Smoke, mTextures) };
	mSceneLayers[static_cast<int>(Layer::LowerAir)]->attachChild(std::move(smokeNode));

	auto propellantNode{ std::make_unique<ParticleNode>(Particle::Type::Propellant, mTextures) };
	mSceneLayers[static_cast<int>(Layer::LowerAir)]->attachChild(std::move(propellantNode));

	auto player{ std::make_unique<Aircraft>(Aircraft::Type::Eagle, mTextures, mFonts) };
	mPlayerAircraft = player.get();
	mPlayerAircraft->setPosition(mSpawnPosition);
	mSceneLayers[static_cast<int>(Layer::UpperAir)]->attachChild(std::move(player));

	addEnemies();
}

void World::addEnemies()
{
	addEnemy(Aircraft::Type::Raptor, 0.f, 500.f);
	addEnemy(Aircraft::Type::Raptor, 0.f, 1000.f);
	addEnemy(Aircraft::Type::Raptor, +100.f, 1150.f);
	addEnemy(Aircraft::Type::Raptor, -100.f, 1150.f);
	addEnemy(Aircraft::Type::Avenger, 70.f, 1500.f);
	addEnemy(Aircraft::Type::Avenger, -70.f, 1500.f);
	addEnemy(Aircraft::Type::Avenger, -70.f, 1710.f);
	addEnemy(Aircraft::Type::Avenger, 70.f, 1700.f);
	addEnemy(Aircraft::Type::Avenger, 30.f, 1850.f);
	addEnemy(Aircraft::Type::Raptor, 300.f, 2200.f);
	addEnemy(Aircraft::Type::Raptor, -300.f, 2200.f);
	addEnemy(Aircraft::Type::Raptor, 0.f, 2200.f);
	addEnemy(Aircraft::Type::Raptor, 0.f, 2500.f);
	addEnemy(Aircraft::Type::Avenger, -300.f, 2700.f);
	addEnemy(Aircraft::Type::Avenger, -300.f, 2700.f);
	addEnemy(Aircraft::Type::Raptor, 0.f, 3000.f);
	addEnemy(Aircraft::Type::Raptor, 250.f, 3250.f);
	addEnemy(Aircraft::Type::Raptor, -250.f, 3250.f);
	addEnemy(Aircraft::Type::Avenger, 0.f, 3500.f);
	addEnemy(Aircraft::Type::Avenger, 0.f, 3700.f);
	addEnemy(Aircraft::Type::Raptor, 0.f, 3800.f);
	addEnemy(Aircraft::Type::Avenger, 0.f, 4000.f);
	addEnemy(Aircraft::Type::Avenger, -200.f, 4200.f);
	addEnemy(Aircraft::Type::Raptor, 200.f, 4200.f);
	addEnemy(Aircraft::Type::Raptor, 0.f, 4400.f);


	std::sort(mEnemySpawnPoints.begin(), mEnemySpawnPoints.end(), [](const SpawnPoint& lhs, const SpawnPoint& rhs)
	{
		return lhs.y < rhs.y;
	});
}

void World::addEnemy(Aircraft::Type type, float relX, float relY)
{
	SpawnPoint spawn(type, mSpawnPosition.x + relX, mSpawnPosition.y - relY);
	mEnemySpawnPoints.push_back(spawn);
}

void World::spawnEnemies()
{
	while (!mEnemySpawnPoints.empty()
		&& mEnemySpawnPoints.back().y > getBattlefieldBounds().top)
	{
		const auto& spawn{ mEnemySpawnPoints.back() };

		auto enemy{ std::make_unique<Aircraft>(spawn.type, mTextures, mFonts) };
		enemy->setPosition(spawn.x, spawn.y);
		enemy->setRotation(180.f);

		mSceneLayers[static_cast<int>(Layer::UpperAir)]->attachChild(std::move(enemy));

		mEnemySpawnPoints.pop_back();
	}
}

void World::destroyEntitiesOutsideView()
{
	Command command;
	command.category = static_cast<unsigned int>(Category::Type::Projectile) | 
		static_cast<unsigned int>(Category::Type::EnemyAircraft);

	command.action = derivedAction<Entity>([this](Entity& e, sf::Time)
	{
		if (!getBattlefieldBounds().intersects(e.getBoundingRect()))
			e.remove();
	});

	mCommandQueue.push(command);
}

void World::guideMissiles()
{
	Command enemyCollector;
	enemyCollector.category = static_cast<unsigned int>(Category::Type::EnemyAircraft);
	enemyCollector.action = derivedAction<Aircraft>([this](Aircraft& enemy, sf::Time)
	{
		if (!enemy.isDestroyed())
			mActiveEnemies.push_back(&enemy);
	});

	Command missileGuider;
	missileGuider.category = static_cast<unsigned int>(Category::Type::AlliedProjectile);
	missileGuider.action = derivedAction<Projectile>([this](Projectile& missile, sf::Time)
	{
		if (!missile.isGuided())
			return;

		float minDistance{ std::numeric_limits<float>::max() };
		Aircraft* closestEnemy{ nullptr };

		for (Aircraft* enemy : mActiveEnemies)
		{
			float enemyDistance = distance(missile, *enemy);

			if (enemyDistance < minDistance)
			{
				closestEnemy = enemy;
				minDistance = enemyDistance;
			}
		}

		if (closestEnemy)
			missile.guideTowards(closestEnemy->getWorldPosition());
	});

	mCommandQueue.push(enemyCollector);
	mCommandQueue.push(missileGuider);
	mActiveEnemies.clear();
}

sf::FloatRect World::getBattlefieldBounds() const
{
	auto bounds{ getViewBounds() };
	bounds.top -= 100.f;
	bounds.height += 100.f;

	return bounds;
}
