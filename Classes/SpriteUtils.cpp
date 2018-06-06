#include "SpriteUtils.h"

using namespace cocos2d;

RefPtr<Sprite> sprite_utils::createSprite(const std::string& filename)
{
    RefPtr<Sprite> sprite = Sprite::create(filename);
    if (!sprite)
    {
        throw std::runtime_error("cannot create sprite from image '" + filename + "'");
    }
    return sprite;
}
