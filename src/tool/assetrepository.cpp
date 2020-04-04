#include <flat.h>

#include "assetrepository.h"

namespace flat::tool
{

static const std::filesystem::path assetMetaDataFileName = ".asset.lua";

AssetRepository::AssetRepository(Flat& flat) :
	m_flat(flat)
{

}

void AssetRepository::addAssetFolder(const std::filesystem::path& assetFolder)
{
	if (std::find(m_assetFolders.begin(), m_assetFolders.end(), assetFolder) == m_assetFolders.end())
	{
		m_assetFolders.push_back(assetFolder);
	}
}

void AssetRepository::scanAllAssets()
{
	clearCache();

	for (const std::filesystem::path& assetFolder : m_assetFolders)
	{
		for (const std::filesystem::directory_entry& directory : std::filesystem::recursive_directory_iterator(assetFolder))
		{
			const std::filesystem::path directoryAssetMetaDataFile = directory.path() / assetMetaDataFileName;
			if (std::filesystem::exists(directoryAssetMetaDataFile))
			{
				addAssetToCache(directory.path());
			}
		}
	}
}

void AssetRepository::clearCache()
{
	m_cachedAssetsByPath.clear();
	m_cachedAssetsByType.clear();
	m_cachedAssets.clear();
}

void AssetRepository::addAssetToCache(const std::filesystem::path& path)
{
	Asset& asset = m_cachedAssets.emplace_back(path);
	asset.loadDetails(m_flat.lua->state);

	m_cachedAssetsByPath[asset.getPath().string()] = &asset;

	std::pair<std::unordered_map<Asset::Type, std::vector<Asset*>>::iterator, bool> cachedAssetsByType = m_cachedAssetsByType.emplace(asset.getType(), std::vector<Asset*>());
	cachedAssetsByType.first->second.push_back(&asset);

	std::cout << "Found asset " << asset.getPath() << " of type " << asset.getType() << std::endl;
}

const Asset* AssetRepository::findAssetFromName(const Asset::Type& type, const std::string name) const
{
	std::unordered_map<Asset::Type, std::vector<Asset*>>::const_iterator it = m_cachedAssetsByType.find(type);
	if (it == m_cachedAssetsByType.cend())
	{
		return nullptr;
	}

	for (const Asset* asset : it->second)
	{
		if (asset->getName() == name)
		{
			return asset;
		}
	}

	return nullptr;
}

} // flat::tool

