#pragma once

#include "cocos2dgame.h"

class Line
{
public:
    Line();
    Line(const cocos2d::Vec2 &vertexA, const cocos2d::Vec2 &vertexB);

    cocos2d::Vec2 vertexA;
    cocos2d::Vec2 vertexB;

    float getDistanceAB() const;

    Line expandAB(const cocos2d::Rect &minBounds) const;

    bool intersectsRect(const cocos2d::Rect &rect) const;

    // Returns true if point is on the left side of the line when looking from A to B.
    bool isPointOnLeft(const cocos2d::Vec2 &point) const;

    // Gets line point by it's x coordinate or returns false if line is vertical.
    bool getLinePointByX(float x, cocos2d::Vec2 &point) const;

    // Gets line point by it's y coordinate or returns false if line is horizontal.
    bool getLinePointByY(float y, cocos2d::Vec2 &point) const;
};
