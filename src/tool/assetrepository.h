#ifndef FLAT_TOOL_ASSETREPOSITORY_H
#define FLAT_TOOL_ASSETREPOSITORY_H

#include <unordered_map>

#include "tool/asset.h"

class Flat;
struct lua_State;

namespace flat::tool
{

class AssetRepository final
{
	public:
		AssetRepository() = delete;
		AssetRepository(const AssetRepository&) = delete;
		AssetRepository(AssetRepository&&) = delete;
		AssetRepository(Flat& flat);
		~AssetRepository() = default;

		void addAssetFolder(const std::filesystem::path& assetFolder);
		void scanAllAssets();

		const Asset* findAssetFromName(const Asset::Type& type, const std::string name) const;

	private:
		void clearCache();
		void addAssetToCache(const std::filesystem::path& path);

	private:
		Flat& m_flat;

		std::vector<std::filesystem::path> m_assetFolders;

		std::vector<Asset> m_cachedAssets;
		std::unordered_map<std::string, Asset*> m_cachedAssetsByPath;
		std::unordered_map<Asset::Type, std::vector<Asset*>> m_cachedAssetsByType;
};

} // flat::tool

#endif // FLAT_TOOL_ASSETREPOSITORY_H