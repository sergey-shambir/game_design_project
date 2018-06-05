#include "WelcomeScene.h"
#include "CustomEvents.h"
#include "GameLevelScene.h"
#include "ViewsFactory.h"

using namespace cocos2d;

namespace
{
RefPtr<Sprite> createSprite(const std::string &filename)
{
	RefPtr<Sprite> sprite = Sprite::create(filename);
	if (!sprite)
	{
		throw std::runtime_error("cannot create sprite from image '" + filename + "'");
	}
	return sprite;
}
} // namespace

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

	auto authors = ViewsFactory::createLargeLabel(
		"Ivan Uskov, Sergey Stepanenko and Sergey Shambir\n"
		"proudly present...");
	authors->setPosition(Vec2{ 0.5f * sceneSize.width, 0.85f * sceneSize.height });
	this->addChild(authors, 0);

	auto title = ViewsFactory::createTitleLabel("Stop the Goats!");
	title->setPosition(Vec2{ 0.5f * sceneSize.width, 0.7f * sceneSize.height });
	this->addChild(title, 0);

	auto decoration = createSprite("res/goats_onstage.png");
	decoration->setScale(2.0f);
	decoration->setPosition(Vec2{ 0.3f * sceneSize.width, 0.4f * sceneSize.height });
	this->addChild(decoration, 0);
}

void WelcomeScene::initMenu()
{
	const Size sceneSize = getContentSize();

	auto level1Button = ViewsFactory::createButton("Play Now!", []() {
		auto scene = GameLevelScene::createScene(GAME_LEVEL1_ID);
		Director::getInstance()->pushScene(TransitionFade::create(0.5f, scene));
	});
	level1Button->setPosition(Vec2{ 0.7f * sceneSize.width, 0.45f * sceneSize.height });
	this->addChild(level1Button, 0);

	auto closeButton = ViewsFactory::createButton("Quit", []() {
		// Close the cocos2d-x game scene and quit the application
		Director::getInstance()->end();
	});
	closeButton->setPosition(Vec2{ 0.7f * sceneSize.width, 0.35f * sceneSize.height });
	this->addChild(closeButton, 0);
}
