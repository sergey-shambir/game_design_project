#pragma once

#include "cocos2dgame.h"
#include <string>

namespace sprite_utils
{
cocos2d::RefPtr<cocos2d::Sprite> createSprite(const std::string &filename);
}
