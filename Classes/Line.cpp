#include "Line.h"
#include <cmath>
#include <limits>
#include <numeric>

using namespace cocos2d;

namespace
{
bool isBetween(float value, float from, float to)
{
    assert(from <= to);
    return (value >= from && value <= to);
}

bool areAlmostEqual(float a, float b)
{
    return std::abs(a - b) < std::numeric_limits<float>::epsilon();
}

float distance(const Vec2 &a, const Vec2 &b)
{
    return (b - a).length();
}

Vec2 perp(const Vec2 &value)
{
    return { -value.y, value.x };
}

float dot(const Vec2 &a, const Vec2 &b)
{
    return a.x * b.x + a.y * b.y;
}
} // namespace

Line::Line()
    : vertexA(0, 0)
    , vertexB(0, 0)
{
}

Line::Line(const Vec2 &vertexA, const Vec2 &vertexB)
    : vertexA(vertexA)
    , vertexB(vertexB)
{
}

float Line::getDistanceAB() const
{
    return distance(vertexA, vertexB);
}

Line Line::expandAB(const cocos2d::Rect& minBounds) const
{
    Vec2 expandedA;
    if (!getLinePointByX(minBounds.getMinX(), expandedA))
    {
        getLinePointByY(minBounds.getMinY(), expandedA);
    }

    Vec2 expandedB;
    if (!getLinePointByX(minBounds.getMaxX(), expandedB))
    {
        getLinePointByY(minBounds.getMaxY(), expandedB);
    }

    return Line(expandedA, expandedB);
}

bool Line::intersectsRect(const Rect &rect) const
{
    const float left = rect.getMinX();
    const float right = rect.getMaxX();
    const float top = rect.getMaxX();
    const float bottom = rect.getMinX();

    Vec2 point = { 0, 0 };
    const bool intersectsLeft = (getLinePointByX(left, point) && isBetween(point.y, bottom, top));
    const bool intersectsRight = (getLinePointByX(right, point) && isBetween(point.y, bottom, top));
    const bool intersectsTop = (getLinePointByY(top, point) && isBetween(point.x, left, right));
    const bool intersectsBottom = (getLinePointByY(bottom, point) && isBetween(point.x, left, right));

    return intersectsLeft || intersectsRight || intersectsTop || intersectsBottom;
}

bool Line::isPointOnLeft(const Vec2 &point) const
{
    const Vec2 ap = point - vertexA;
    const Vec2 ab = vertexB - vertexA;

    return (dot(ap, perp(ab)) < 0);
}

bool Line::getLinePointByX(float x, Vec2 &point) const
{
    // Cannot get point by X if line is vertical.
	if (areAlmostEqual(vertexA.x, vertexB.x))
    {
        return false;
    }

    if (vertexA.x < vertexB.x)
    {
        float t = (x - vertexA.x) / (vertexB.x - vertexA.x);
        point = vertexA * (1 - t) + vertexB * t;
    }
    else
    {
        float t = (x - vertexB.x) / (vertexA.x - vertexB.x);
        point = vertexB * (1 - t) + vertexA * t;
    }
    return true;
}

bool Line::getLinePointByY(float y, Vec2 &point) const
{
    // Cannot get point by Y if line is horizontal.
	if (areAlmostEqual(vertexA.y, vertexB.y))
    {
        return false;
    }

    if (vertexA.y < vertexB.y)
    {
        float t = (y - vertexA.y) / (vertexB.y - vertexA.y);
        point = vertexA * (1 - t) + vertexB * t;
    }
    else
    {
        float t = (y - vertexB.y) / (vertexA.y - vertexB.y);
        point = vertexB * (1 - t) + vertexA * t;
    }
    return true;
}
