#include <algorithm>

#include "path.h"

namespace flat::sharp::ai::navigation
{

void Path::appendPath(const Path& path)
{
	m_points.insert(m_points.end(), path.m_points.begin() + 1, path.m_points.end());
}

void Path::appendPoint(const Vector2& point)
{
	m_points.push_back(point);
}

void Path::prependPoint(const Vector2& point)
{
	m_points.insert(m_points.begin(), point);
}

void Path::setPoint(int pointIndex, const Vector2& point)
{
	m_points.at(pointIndex) = point;
}

void Path::removePoint(int pointIndex)
{
	m_points.erase(m_points.begin() + pointIndex - 1);
}

std::vector<flat::Vector2> Path::getUniqueCellPositions() const
{
	std::vector<flat::Vector2> uniqueCellPositions = m_points;
	if (uniqueCellPositions.size() > 0)
	{
		// remove duplicates
		uniqueCellPositions.erase(std::unique(uniqueCellPositions.begin(), uniqueCellPositions.end()), uniqueCellPositions.end());
	}
		return uniqueCellPositions;
}

} // flat::sharp::ai::navigation

