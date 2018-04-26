#include "Container.hpp"

namespace GUI
{
	void Container::pack(std::shared_ptr<GUI::Component> component)
	{
		mChildren.push_back(component);

		if (!hasSelection() && component->isSelectable())
			select(mChildren.size() - 1);
	}

	void Container::handleEvent(const sf::Event& event)
	{
		if (hasSelection() && mChildren[mSelectedChild]->isActive())
		{
			mChildren[mSelectedChild]->handleEvent(event);
		}
		else if (event.type == sf::Event::KeyReleased)
		{
			if (event.key.code == sf::Keyboard::W || event.key.code == sf::Keyboard::Up)
			{
				selectPrevious();
			}
			else if (event.key.code == sf::Keyboard::S || event.key.code == sf::Keyboard::Down)
			{
				selectNext();
			}
			else if (event.key.code == sf::Keyboard::Return || event.key.code == sf::Keyboard::Space)
			{
				if (hasSelection())
					mChildren[mSelectedChild]->activate();
			}
		}
	}

	void Container::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		states.transform *= getTransform();

		for (const auto& child : mChildren)
			target.draw(*child, states);
	}

	void Container::select(std::size_t index)
	{
		if (mChildren[index]->isSelectable())
		{
			if (hasSelection())
				mChildren[mSelectedChild]->deselect();

			mChildren[index]->select();
			mSelectedChild = index;
		}
	}

	void Container::selectNext()
	{
		if (!hasSelection())
			return;

		int next{ mSelectedChild };
	
		do
		{
			next = (next + 1) % mChildren.size();
		} while (!mChildren[next]->isSelectable());

		select(next);
	}

	void Container::selectPrevious()
	{
		if (!hasSelection())
			return;

		int prev{ mSelectedChild };
		do
		{
			prev = (prev + mChildren.size() - 1) % mChildren.size();
		} while (!mChildren[prev]->isSelectable());

		select(prev);
	}

}
