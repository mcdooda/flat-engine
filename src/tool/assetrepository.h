#ifndef FLAT_TOOL_ASSETREPOSITORY_H
#define FLAT_TOOL_ASSETREPOSITORY_H

#include <unordered_map>

#include "tool/asset.h"

class Flat;
struct lua_State;

namespace flat::tool
{

enum AssetIndex
{

};

enum AssetDirectoryIndex
{
	INVALID_ASSET_DIRECTORY = 0xFFFFFFFF
};

inline bool isValidAssetDirectory(AssetDirectoryIndex assetDirectoryIndex)
{
	return assetDirectoryIndex != AssetDirectoryIndex::INVALID_ASSET_DIRECTORY;
}

class AssetRepository final
{
	private:
		struct Directory
		{
			std::filesystem::path path;
			AssetDirectoryIndex parentDirectoryIndex = AssetDirectoryIndex::INVALID_ASSET_DIRECTORY;
			std::vector<AssetDirectoryIndex> directoryIndices;
			std::vector<AssetIndex> assetIndices;
		};

	public:
		AssetRepository() = delete;
		AssetRepository(const AssetRepository&) = delete;
		AssetRepository(AssetRepository&&) = delete;
		AssetRepository(Flat& flat);
		~AssetRepository() = default;

		void addAssetDirectory(const std::filesystem::path& assetDirectory);
		void scanAllAssets();

		const Asset* findAssetFromName(const Asset::Type& type, const Asset::Name& name) const;
		std::vector<const Asset*> searchAllFromName(const Asset::Name& name) const;

		// lua api
		std::vector<std::string> getDirectories(const std::string& path) const;
		std::vector<const Asset*> getAssets(const std::string& path) const;
		bool getParentDirectory(const std::string& path, std::string& parentPath) const;

	private:
		static bool isValidDirectory(const std::filesystem::path& path);

		void clearCache();
		void scanDirectoryRecursive(AssetDirectoryIndex parentDirectoryIndex, const std::filesystem::path& assetDirectory);
		void addAssetToCache(AssetDirectoryIndex parentDirectoryIndex, const std::filesystem::path& path);
		AssetDirectoryIndex addDirectoryToCache(const std::filesystem::path& path);
		AssetDirectoryIndex addSubDirectoryToCache(AssetDirectoryIndex parentDirectoryIndex, const std::filesystem::path& path);

	private:
		Flat& m_flat;

		std::vector<std::filesystem::path> m_assetDirectories;

		std::vector<Asset> m_cachedAssets;
		std::unordered_map<std::string, AssetIndex> m_cachedAssetsByPath;
		std::unordered_map<Asset::Type, std::vector<AssetIndex>> m_cachedAssetsByType;

		std::vector<Directory> m_cachedDirectories;
		std::unordered_map<std::string, AssetDirectoryIndex> m_cachedDirectoriesByPath;
};

} // flat::tool

#endif // FLAT_TOOL_ASSETREPOSITORY_H