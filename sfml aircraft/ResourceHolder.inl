#pragma once

template <typename Resource, typename Identifier>
void ResourceHolder<Resource, Identifier>::load(Identifier id, const std::string_view filename)
{
	auto resource{ std::make_unique<Resource>() };
	if (!resource->loadFromFile(filename.data()))
		throw std::runtime_error("ResourceHolder::load - Failed to load " + std::string(filename));

	insertResource(id, std::move(resource));
}

template <typename Resource, typename Identifier>
template <typename Parameter>
void ResourceHolder<Resource, Identifier>::load(Identifier id, const std::string& filename, const Parameter& secondParam)
{
	auto resource{ std::make_unique<Resource>() };
	if (!resource->loadFromFile(filename, secondParam))
		throw std::runtime_error("ResourceHolder::load - Failed to load " + filename);

	insertResource(id, std::move(resource));
}

template <typename Resource, typename Identifier>
Resource& ResourceHolder<Resource, Identifier>::get(Identifier id)
{
	auto found{ mResourceMap.find(id) };
	assert(found != mResourceMap.end());

	return *found->second;
}

template <typename Resource, typename Identifier>
const Resource& ResourceHolder<Resource, Identifier>::get(Identifier id) const
{
	auto found{ mResourceMap.find(id) };
	assert(found != mResourceMap.end());

	return *found->second;
}

template <typename Resource, typename Identifier>
void ResourceHolder<Resource, Identifier>::insertResource(Identifier id, std::unique_ptr<Resource> resource)
{
	auto inserted{ mResourceMap.insert({ id, std::move(resource) }) };
	assert(inserted.second);
}