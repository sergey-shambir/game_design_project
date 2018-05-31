#include "WelcomeScene.h"
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

	auto level1Item = ViewsFactory::createMenuItemLabel("Play on level 1", [](auto &&) {
		// TODO: pass level1.tmx to scene.
		auto scene = GameLevelScene::createScene();
		Director::getInstance()->pushScene(TransitionFade::create(0.5f, scene));
	});

	auto level2Item = ViewsFactory::createMenuItemLabel("Play on level 2", [](auto &&) {
		// TODO: pass level1.tmx to scene.
		auto scene = GameLevelScene::createScene();
		Director::getInstance()->pushScene(TransitionFade::create(0.5f, scene));
	});

	auto level3Item = ViewsFactory::createMenuItemLabel("Play on level 3", [](auto &&) {
		// TODO: pass level1.tmx to scene.
		auto scene = GameLevelScene::createScene();
		Director::getInstance()->pushScene(TransitionFade::create(0.5f, scene));
	});

	auto closeItem = ViewsFactory::createMenuItemLabel("Quit", [](auto &&) {
		//Close the cocos2d-x game scene and quit the application
		Director::getInstance()->end();
	});

	auto menu = ViewsFactory::createVerticalMenu(level1Item, level2Item, level3Item, closeItem);
	menu->setPosition(menuPos);
	this->addChild(menu, 1);
}
