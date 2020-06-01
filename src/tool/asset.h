#ifndef FLAT_TOOL_ASSET_H
#define FLAT_TOOL_ASSET_H

#include <vector>
#include <filesystem>

struct lua_State;

namespace flat::tool
{

class Asset final
{
	public:
		using Type = std::string;
		using Name = std::string;

	public:
		Asset(const std::filesystem::path& path);

		inline const std::filesystem::path& getPath() const { return m_path; }
		inline Name getName() const { return m_path.stem().string(); }

		inline const Type& getType() const { return m_type; }

		void loadDetails(lua_State* L);

	private:
		std::filesystem::path getMetaDataFilePath() const;

	private:
		std::filesystem::path m_path;
		Type m_type;
};

} // flat::tool

#endif // FLAT_TOOL_ASSET_H