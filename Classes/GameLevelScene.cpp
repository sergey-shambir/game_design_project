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
	m_map = GameLevelMap::create("res/goats_level1.tmx");
	this->addChild(m_map, 1);

	m_hud = BoundariesLayer::create(getContentSize(), *m_map);
	this->addChild(m_hud, 2);
}

void GameLevelScene::preloadResources()
{
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("res/objects.plist");
}
