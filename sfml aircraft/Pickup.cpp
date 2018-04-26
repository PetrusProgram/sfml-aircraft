#include "Pickup.hpp"
#include "DataTables.hpp"
#include "Utility.hpp"
#include "Category.hpp"
#include "Command.hpp"
#include "ResourceHolder.hpp"

namespace
{
	const std::vector<PickupData> Table = initializePickupData();
}

Pickup::Pickup(Type type, const TextureHolder& textures) : Entity(1), mType(type), 
	mSprite(textures.get(Table[static_cast<int>(type)].texture), Table[static_cast<int>(type)].textureRect)
{
	centerOrigin(mSprite);
}

void Pickup::apply(Aircraft& player) const
{
	Table[static_cast<int>(mType)].action(player);
}


