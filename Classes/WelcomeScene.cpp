#include "WelcomeScene.h"
#include "CustomEvents.h"
#include "GameLevelScene.h"
#include "ViewsFactory.h"

using namespace cocos2d;

Scene *WelcomeScene::createScene()
{
	return WelcomeScene::create();
}

void WelcomeScene::tryInit()
{
	const Size sceneSize = getContentSize();
	const Vec2 menuPos = Vec2{ sceneSize.width / 2.0f, sceneSize.height / 2.0f };

	auto level1Item = ViewsFactory::createMenuItemLabel("Play Now!", [](auto &&) {
		auto scene = GameLevelScene::createScene(GAME_LEVEL1_ID);
		Director::getInstance()->pushScene(TransitionFade::create(0.5f, scene));
	});

	auto closeItem = ViewsFactory::createMenuItemLabel("Quit", [](auto &&) {
		//Close the cocos2d-x game scene and quit the application
		Director::getInstance()->end();
	});

#if 0
	auto level2Item = ViewsFactory::createMenuItemLabel("Play on level 2", [](auto &&) {
		auto scene = GameLevelScene::createScene(GAME_LEVEL2_ID);
		Director::getInstance()->pushScene(TransitionFade::create(0.5f, scene));
	});

	auto level3Item = ViewsFactory::createMenuItemLabel("Play on level 3", [](auto &&) {
		auto scene = GameLevelScene::createScene(GAME_LEVEL3_ID);
		Director::getInstance()->pushScene(TransitionFade::create(0.5f, scene));
	});

	auto debugLevelItem = ViewsFactory::createMenuItemLabel("Run debug level", [](auto &&) {
		auto scene = GameLevelScene::createScene(GAME_LEVEL_DEBUG_ID);
		Director::getInstance()->pushScene(TransitionFade::create(0.5f, scene));
	});

	auto menu = ViewsFactory::createVerticalMenu(level1Item, level2Item, level3Item, debugLevelItem, closeItem);
#else
	auto menu = ViewsFactory::createVerticalMenu(level1Item, closeItem);
#endif

	menu->setPosition(menuPos);
	this->addChild(menu, 1);
}
