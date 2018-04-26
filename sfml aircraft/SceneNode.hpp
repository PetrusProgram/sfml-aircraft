#pragma once
#include "Category.hpp"
#include "sfml\Graphics.hpp"
#include <vector>
#include <set>
#include <memory>
#include <utility>

struct Command;
class CommandQueue;

class SceneNode : public sf::Transformable, public sf::Drawable
{
public:
	using Pair = std::pair<SceneNode*, SceneNode*>;

public:
	SceneNode() = default;
	virtual ~SceneNode() = default;
	explicit SceneNode(Category::Type category) : mDefaultCategory(category) {}
	SceneNode(const SceneNode& other) = delete;
	SceneNode& operator=(const SceneNode& other) = delete;
	void attachChild(std::unique_ptr<SceneNode> child);
	std::unique_ptr<SceneNode> detachChild(const SceneNode& node);

	void update(sf::Time dt, CommandQueue& commands);

	sf::Vector2f getWorldPosition() const {	return getWorldTransform() * sf::Vector2f(); }
	sf::Transform getWorldTransform() const;

	void onCommand(const Command& command, sf::Time dt);
	virtual unsigned int getCategory() const { return static_cast<unsigned int>(mDefaultCategory); }

	void checkSceneCollision(SceneNode& sceneGraph, std::set<Pair>& collisionPairs);
	void checkNodeCollision(SceneNode& node, std::set<Pair>& collisionPairs);
	void removeWrecks();
	virtual sf::FloatRect getBoundingRect() const {	return sf::FloatRect(); }
	virtual bool isMarkedForRemoval() const { return isDestroyed(); }
	virtual bool isDestroyed() const { return false; }

private:
	virtual void updateCurrent(sf::Time dt, CommandQueue& commands) {}
	void updateChildren(sf::Time dt, CommandQueue& commands);

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const {}
	void drawChildren(sf::RenderTarget& target, sf::RenderStates states) const;
	void drawBoundingRect(sf::RenderTarget& target, sf::RenderStates states) const;


private:
	std::vector<std::unique_ptr<SceneNode>>	mChildren;
	SceneNode* mParent{ nullptr };
	Category::Type mDefaultCategory{ Category::Type::None };
};

bool collision(const SceneNode& lhs, const SceneNode& rhs);
float distance(const SceneNode& lhs, const SceneNode& rhs);