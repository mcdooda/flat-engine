#ifndef FLAT_SHARP_AI_NAVIGATION_PATH_H
#define FLAT_SHARP_AI_NAVIGATION_PATH_H

#include <vector>
#include "misc/vector.h"

namespace flat::sharp::ai::navigation
{

class Path
{
    friend class Pathfinder;
    public:
        Path() = default;
        Path(Path&& path) = default;
        Path(Path& path) = default;

        inline const std::vector<Vector2>& getPoints() const { return m_points; }
        inline const flat::Vector2& getPoint(int index) const { return m_points[index]; }
        inline int getPointsCount() const { return static_cast<int>(m_points.size()); }
        inline const flat::Vector2& getLastPoint() const { return m_points.back(); }
        inline const flat::Vector2& getFirstPoint() const { return m_points.front(); }
        inline bool isEmpty() const { return m_points.empty(); }

        std::vector<Vector2> getUniqueCellPositions() const;

        inline void clear() { return m_points.clear(); }

        void appendPath(const Path& path);

        void setPoint(int pointIndex, const Vector2& point);
        void appendPoint(const Vector2& point);
        void prependPoint(const Vector2& point);
        void removePoint(int pointIndex);

    private:
        std::vector<Vector2> m_points;
};

} // flat::sharp::ai::navigation

#endif // FLAT_SHARP_AI_NAVIGATION_PATH_H