#pragma once

#include "cocos2dgame.h"
#include <vector>

class IGameLevelMap
{
public:
    virtual ~IGameLevelMap() = default;

	virtual std::string getLevelId() const = 0;
	virtual unsigned getBoundaryCount() const = 0;
	virtual unsigned getEstimatedSpentSeconds() const = 0;
    virtual std::vector<cocos2d::Rect> getAllObstacles() const = 0;
    virtual std::vector<cocos2d::Rect> getAnimalsRects() const = 0;
    virtual std::vector<cocos2d::Rect> getPlantsRects() const = 0;
};
