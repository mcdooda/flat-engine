#include <flat.h>

#include "assetrepository.h"

namespace flat::tool
{

static const std::filesystem::path assetMetaDataFileName = ".asset.lua";

AssetRepository::AssetRepository(Flat& flat) :
	m_flat(flat)
{

}

void AssetRepository::addAssetDirectory(const std::filesystem::path& assetDirectory)
{
	FLAT_ASSERT(isValidDirectory(assetDirectory));
	if (std::find(m_assetDirectories.begin(), m_assetDirectories.end(), assetDirectory) == m_assetDirectories.end())
	{
		m_assetDirectories.push_back(assetDirectory);
	}
}

void AssetRepository::scanAllAssets()
{
	clearCache();

	for (const std::filesystem::path& assetDirectory : m_assetDirectories)
	{
		scanDirectoryRecursive(AssetDirectoryIndex::INVALID_ASSET_DIRECTORY, assetDirectory);
	}
}

bool AssetRepository::isValidDirectory(const std::filesystem::path& path)
{
	return std::filesystem::is_directory(path) && !std::filesystem::exists(path / assetMetaDataFileName);
}

void AssetRepository::clearCache()
{
	m_cachedAssets.clear();
	m_cachedAssetsByPath.clear();
	m_cachedAssetsByType.clear();
	m_cachedDirectories.clear();
	m_cachedDirectoriesByPath.clear();
}

void AssetRepository::scanDirectoryRecursive(AssetDirectoryIndex parentDirectoryIndex, const std::filesystem::path& assetDirectory)
{
	FLAT_ASSERT(std::filesystem::is_directory(assetDirectory));
	const std::filesystem::path directoryAssetMetaDataFile = assetDirectory / assetMetaDataFileName;
	if (std::filesystem::exists(directoryAssetMetaDataFile))
	{
		addAssetToCache(parentDirectoryIndex, assetDirectory);
	}
	else
	{
		const AssetDirectoryIndex directoryIndex = addSubDirectoryToCache(parentDirectoryIndex, assetDirectory);
		for (const std::filesystem::directory_entry& directory : std::filesystem::directory_iterator(assetDirectory))
		{
			if (std::filesystem::is_directory(directory))
			{
				scanDirectoryRecursive(directoryIndex, directory);
			}
		}
	}
}

void AssetRepository::addAssetToCache(AssetDirectoryIndex parentDirectoryIndex, const std::filesystem::path& path)
{
	const AssetIndex assetIndex = static_cast<AssetIndex>(m_cachedAssets.size());

	Asset& asset = m_cachedAssets.emplace_back(path);
	asset.loadDetails(m_flat.lua->state);

	m_cachedAssetsByPath[asset.getPath().string()] = assetIndex;

	std::pair<std::unordered_map<Asset::Type, std::vector<AssetIndex>>::iterator, bool> cachedAssetsByType = m_cachedAssetsByType.emplace(asset.getType(), std::vector<AssetIndex>());
	cachedAssetsByType.first->second.push_back(assetIndex);

	Directory& parentDirectory = m_cachedDirectories.at(parentDirectoryIndex);
	parentDirectory.assetIndices.push_back(assetIndex);
}

AssetDirectoryIndex AssetRepository::addDirectoryToCache(const std::filesystem::path& path)
{
	FLAT_ASSERT(isValidDirectory(path));

	const AssetDirectoryIndex directoryIndex = static_cast<AssetDirectoryIndex>(m_cachedDirectories.size());

	Directory& directory = m_cachedDirectories.emplace_back();
	directory.path = path;

	std::pair<std::unordered_map<std::string, AssetDirectoryIndex>::iterator, bool> pair;
	pair = m_cachedDirectoriesByPath.emplace(
		std::piecewise_construct,
		std::forward_as_tuple(path.string()),
		std::forward_as_tuple(directoryIndex)
	);
	FLAT_ASSERT(pair.second);

	return directoryIndex;
}

AssetDirectoryIndex AssetRepository::addSubDirectoryToCache(AssetDirectoryIndex parentDirectoryIndex, const std::filesystem::path& path)
{
	FLAT_ASSERT(isValidDirectory(path));

	const AssetDirectoryIndex directoryIndex = static_cast<AssetDirectoryIndex>(m_cachedDirectories.size());

	Directory& directory = m_cachedDirectories.emplace_back();
	directory.path = path;

	std::pair<std::unordered_map<std::string, AssetDirectoryIndex>::iterator, bool> pair;
	pair = m_cachedDirectoriesByPath.emplace(
		std::piecewise_construct,
		std::forward_as_tuple(path.string()),
		std::forward_as_tuple(directoryIndex)
	);
	FLAT_ASSERT(pair.second);

	if (isValidAssetDirectory(parentDirectoryIndex))
	{
		Directory& parentDirectory = m_cachedDirectories.at(parentDirectoryIndex);
		parentDirectory.directoryIndices.push_back(directoryIndex);

		directory.parentDirectoryIndex = parentDirectoryIndex;
	}

	return directoryIndex;
}

const Asset* AssetRepository::findAssetFromName(const Asset::Type& type, const Asset::Name& name) const
{
	std::unordered_map<Asset::Type, std::vector<AssetIndex>>::const_iterator it = m_cachedAssetsByType.find(type);
	if (it == m_cachedAssetsByType.cend())
	{
		return nullptr;
	}

	for (AssetIndex assetIndex : it->second)
	{
		const Asset* asset = &m_cachedAssets.at(assetIndex);
		if (asset->getName() == name)
		{
			return asset;
		}
	}

	return nullptr;
}

std::vector<const Asset*> AssetRepository::searchAllFromName(const Asset::Name& name) const
{
	std::vector<const Asset*> assets;
	for (const Asset& asset : m_cachedAssets)
	{
		if (asset.getName().find(name) != std::string::npos)
		{
			assets.push_back(&asset);
		}
	}
	return assets;
}

std::vector<std::string> AssetRepository::getDirectories(const std::string& path) const
{
	const AssetDirectoryIndex directoryIndex = m_cachedDirectoriesByPath.at(path);
	const std::vector<AssetDirectoryIndex>& subDirectoryIndices = m_cachedDirectories.at(directoryIndex).directoryIndices;
	std::vector<std::string> subDirectoryPaths;
	subDirectoryPaths.reserve(subDirectoryIndices.size());
	for (AssetDirectoryIndex subDirectoryIndex : subDirectoryIndices)
	{
		const Directory& directory = m_cachedDirectories.at(subDirectoryIndex);
		subDirectoryPaths.push_back(directory.path.string());
	}
	return subDirectoryPaths;
}

std::vector<const Asset*> AssetRepository::getAssets(const std::string& path) const
{
	const AssetDirectoryIndex directoryIndex = m_cachedDirectoriesByPath.at(path);
	const std::vector<AssetIndex>& assetIndices = m_cachedDirectories.at(directoryIndex).assetIndices;
	std::vector<const Asset*> assets;
	assets.reserve(assetIndices.size());
	for (AssetIndex assetIndex : assetIndices)
	{
		const Asset& asset = m_cachedAssets.at(assetIndex);
		assets.push_back(&asset);
	}
	return assets;
}

bool AssetRepository::getParentDirectory(const std::string& path, std::string& parentPath) const
{
	const AssetDirectoryIndex directoryIndex = m_cachedDirectoriesByPath.at(path);
	const AssetDirectoryIndex parentDirectoryIndex = m_cachedDirectories[directoryIndex].parentDirectoryIndex;
	if (isValidAssetDirectory(parentDirectoryIndex))
	{
		parentPath = m_cachedDirectories[parentDirectoryIndex].path.string();
		return true;
	}
	else
	{
		return false;
	}
}

} // flat::tool

