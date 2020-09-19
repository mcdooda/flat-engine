#include <algorithm>
#include <filesystem>

#include "path.h"

namespace flat::lua
{

void formatPathToLua(std::string& path)
{
	constexpr char separator = std::filesystem::path::preferred_separator;
	if constexpr (separator != '/')
	{
		std::replace(path.begin(), path.end(), separator, '/');
	}
}

void formatPathFromLua(std::string& path)
{
	constexpr char separator = std::filesystem::path::preferred_separator;
	if constexpr (separator != '/')
	{
		std::replace(path.begin(), path.end(), '/', separator);
	}
}

} // flat::lua