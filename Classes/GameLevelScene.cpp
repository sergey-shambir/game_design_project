#include "GameLevelScene.h"

using namespace cocos2d;
using namespace std::literals;

Scene *GameLevelScene::createScene()
{
	return GameLevelScene::create();
}

void GameLevelScene::tryInit()
{
	preloadResources();

	const Size sceneSize = getContentSize();
	const Vec2 goatPos = Vec2{ sceneSize.width / 2.0f, sceneSize.height / 2.0f };

#if 0
	// TODO: move this code to SpritesFactory
	RefPtr<Sprite> goatSprite = createWithSpriteFrameNameSafe("goat_6.png");
	goatSprite->setPosition(goatPos);
	this->addChild(goatSprite, 1);
#endif

	m_map = GameLevelMap::create("res/goats_level1.tmx");
	this->addChild(m_map, 1);
}

void GameLevelScene::preloadResources()
{
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("res/objects.plist");
}
