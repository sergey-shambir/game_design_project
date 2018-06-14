#include "GameIntroLayer.h"
#include "CustomEvents.h"
#include "ViewsFactory.h"
#include "LocalizableStrings.h"

using namespace cocos2d;

namespace
{
const cocos2d::Color4F kGameIntroSplashColor = { 0.0f, 0.0f, 0.0f, 0.75f };
} // namespace

GameIntroLayer *GameIntroLayer::create(const Size &contentSize, const std::string &levelId)
{
    RefPtr<GameIntroLayer> layer = new (std::nothrow) GameIntroLayer;
    if (layer && layer->init(contentSize, levelId))
    {
        layer->autorelease();
        return layer;
    }
    return nullptr;
}

bool GameIntroLayer::init(const Size &contentSize, const std::string &levelId)
{
    if (!DrawNode::init())
    {
        return false;
    }
	setContentSize(contentSize);

    try
	{
		const Vec2 size{ contentSize.width, contentSize.height };
        this->drawSolidRect(Vec2{ 0, 0 }, size, kGameIntroSplashColor);

		std::string text = l10n::kGameIntro;
        RefPtr<Label> label = ViewsFactory::createLargeLabel(text);
		label->setPosition(Vec2{ 0.5f * size.x, 0.6f * size.y });
        this->addChild(label, 1);

		RefPtr<ui::Button> finishBtn = ViewsFactory::createButton(l10n::kGameIntroCloseButton, [this, levelId] {
            auto event = CustomEvents::make(EVENT_START_LEVEL, LevelEventData::create(levelId));
            getEventDispatcher()->dispatchEvent(event);
        });
		finishBtn->setPosition(Vec2{ 0.5f * size.x, 0.3f * size.y });
        this->addChild(finishBtn, 2);
    }
    catch (const std::exception &ex)
    {
        CCLOG("init failed: %s", ex.what());
        return false;
    }

    return true;
}
