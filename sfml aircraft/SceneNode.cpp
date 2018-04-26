#include "SceneNode.hpp"
#include "Utility.hpp"
#include "Command.hpp"
#include <algorithm>
#include <cassert>
#include <cmath>

void SceneNode::attachChild(std::unique_ptr<SceneNode> child)
{
	child->mParent = this;
	mChildren.push_back(std::move(child));
}

std::unique_ptr<SceneNode> SceneNode::detachChild(const SceneNode& node)
{
	auto found{ std::find_if(mChildren.begin(), mChildren.end(),
		[&](std::unique_ptr<SceneNode>& p) { return p.get() == &node; }) };
	assert(found != mChildren.end());

	auto result{ std::move(*found) };
	result->mParent = nullptr;
	mChildren.erase(found);
	return result;
}

void SceneNode::update(sf::Time dt, CommandQueue& commands)
{
	updateCurrent(dt, commands);
	updateChildren(dt, commands);
}

void SceneNode::updateChildren(sf::Time dt, CommandQueue& commands)
{
	for (auto& child : mChildren)
		child->update(dt, commands);
}

void SceneNode::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();

	drawCurrent(target, states);
	drawChildren(target, states);

	//drawBoundingRect(target, states);
}

void SceneNode::drawChildren(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (const auto& child : mChildren)
		child->draw(target, states);
}

void SceneNode::drawBoundingRect(sf::RenderTarget& target, sf::RenderStates) const
{
	auto rect{ getBoundingRect() };

	sf::RectangleShape shape;
	shape.setPosition(sf::Vector2f(rect.left, rect.top));
	shape.setSize(sf::Vector2f(rect.width, rect.height));
	shape.setFillColor(sf::Color::Transparent);
	shape.setOutlineColor(sf::Color::Green);
	shape.setOutlineThickness(1.f);

	target.draw(shape);
}

sf::Transform SceneNode::getWorldTransform() const
{
	auto transform{ sf::Transform::Identity };

	for (const SceneNode* node = this; node != nullptr; node = node->mParent)
		transform = node->getTransform() * transform;

	return transform;
}

void SceneNode::onCommand(const Command& command, sf::Time dt)
{
	if (command.category & getCategory())
		command.action(*this, dt);

	for (auto& child : mChildren)
		child->onCommand(command, dt);
}

void SceneNode::checkSceneCollision(SceneNode& sceneGraph, std::set<Pair>& collisionPairs)
{
	if (sceneGraph.getCategory() && static_cast<unsigned int>(Category::Type::collision))
		checkNodeCollision(sceneGraph, collisionPairs);
	
	for (auto& child : sceneGraph.mChildren)
		checkSceneCollision(*child, collisionPairs);
}

void SceneNode::checkNodeCollision(SceneNode& node, std::set<Pair>& collisionPairs)
{
	if (this != &node && collision(*this, node) && !isDestroyed() && !node.isDestroyed())
		collisionPairs.insert(std::minmax(this, &node));

	for (auto& child : mChildren)
		child->checkNodeCollision(node, collisionPairs);
}

void SceneNode::removeWrecks()
{	
	const auto wreckfieldBegin{ std::remove_if(mChildren.begin(), mChildren.end(),
		[](const std::unique_ptr<SceneNode>& test) {return test->isMarkedForRemoval(); }) };

		mChildren.erase(wreckfieldBegin, mChildren.end());
	
	std::for_each(mChildren.begin(), mChildren.end(), [](const std::unique_ptr<SceneNode>& test) 
	{ test->removeWrecks(); });
}

bool collision(const SceneNode& lhs, const SceneNode& rhs)
{
	return lhs.getBoundingRect().intersects(rhs.getBoundingRect());
}

float distance(const SceneNode& lhs, const SceneNode& rhs)
{
	return length(lhs.getWorldPosition() - rhs.getWorldPosition());
}
