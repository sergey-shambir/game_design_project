#pragma once
#include "cocos2dgame.h"
#include <string>

class GameIntroLayer : public cocos2d::DrawNode
{
public:
    static GameIntroLayer *create(const cocos2d::Size &contentSize, const std::string& levelId);
    bool init(const cocos2d::Size &contentSize, const std::string& levelId);
};
