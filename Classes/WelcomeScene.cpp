#include "WelcomeScene.h"
#include "CustomEvents.h"
#include "GameLevelScene.h"
#include "ViewsFactory.h"
#include "ScoreManager.h"
#include "SpriteUtils.h"
#include "LocalizableStrings.h"

using namespace cocos2d;

Scene *WelcomeScene::createScene()
{
	return WelcomeScene::create();
}

void WelcomeScene::tryInit()
{
	initBackground();
	initMenu();
}

void WelcomeScene::initBackground()
{
	const Size sceneSize = getContentSize();

	auto background = ViewsFactory::createColoredLayer(Color4B{ 15, 90, 50, 255 }, sceneSize);
	this->addChild(background, 0);

	auto authors = ViewsFactory::createLargeLabel(l10n::kGameAuthors);
	authors->setPosition(Vec2{ 0.5f * sceneSize.width, 0.85f * sceneSize.height });
	this->addChild(authors, 0);

	auto title = ViewsFactory::createTitleLabel(l10n::kGameTitle);
	title->setPosition(Vec2{ 0.5f * sceneSize.width, 0.7f * sceneSize.height });
	this->addChild(title, 0);

	auto decoration = sprite_utils::createSprite("res/goats_onstage.png");
	decoration->setScale(2.0f);
	decoration->setPosition(Vec2{ 0.3f * sceneSize.width, 0.4f * sceneSize.height });
	this->addChild(decoration, 0);
}

void WelcomeScene::initMenu()
{
	const Size sceneSize = getContentSize();

	auto level1Button = ViewsFactory::createButton(l10n::kPlayNow, []() {
		ScoreManager::getInstance().updateBeforeFirstLevel();
		auto scene = GameLevelScene::createScene(GAME_LEVEL1_ID);
		Director::getInstance()->pushScene(TransitionFade::create(0.5f, scene));
	});
	level1Button->setPosition(Vec2{ 0.7f * sceneSize.width, 0.5f * sceneSize.height });
	this->addChild(level1Button, 0);

	auto closeButton = ViewsFactory::createButton(l10n::kButtonQuit, []() {
		// Close the cocos2d-x game scene and quit the application
		Director::getInstance()->end();
	});
	closeButton->setPosition(Vec2{ 0.7f * sceneSize.width, 0.3f * sceneSize.height });
	this->addChild(closeButton, 0);
}
