#pragma once

#include "cocos2dgame.h"

class AbstractScene : public cocos2d::Scene
{
public:
    virtual void tryInit() = 0;

	bool init() final;
};
