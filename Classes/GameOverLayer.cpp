#include "GameOverLayer.h"
#include "CustomEvents.h"
#include "ScoreManager.h"
#include "ViewsFactory.h"
#include <memory>

using namespace cocos2d;

namespace
{
const cocos2d::Color4F kGameOverSplashColor = { 0.0f, 0.0f, 0.0f, 0.75f };

Vec2 size2vec(const Size &size)
{
	return Vec2{ size.width, size.height };
}
} // namespace

GameOverLayer *GameOverLayer::create(const Size& contentSize, IGameLevelMap& map, GameStatus status)
{
	RefPtr<GameOverLayer> layer = new (std::nothrow) GameOverLayer;
	if (layer && layer->init(contentSize, map, status))
	{
		layer->autorelease();
		return layer;
	}
	return nullptr;
}

bool GameOverLayer::init(const Size& contentSize, IGameLevelMap& map, GameStatus status)
{
	if (!DrawNode::init())
	{
		return false;
	}
	setContentSize(contentSize);
	m_map = &map;

	try
	{
		const Vec2 size{ size2vec(getContentSize()) };
		this->drawSolidRect(Vec2{ 0, 0 }, size, kGameOverSplashColor);

		if (status == GameStatus::Win)
		{
			auto event = CustomEvents::make(EVENT_WIN_ON_LEVEL, LevelEventData::create(m_map->getLevelId()));
			getEventDispatcher()->dispatchEvent(event);

			std::string text = "Game Over\nCongratulations, you won!\n"
							   "Your score: "
				+ std::to_string(ScoreManager::getInstance().getScore());
			RefPtr<Label> label = ViewsFactory::createLargeLabel(text);
			label->setPosition(Vec2{ 0.5f * size.x, 0.7f * size.y });
			this->addChild(label, 1);

			RefPtr<ui::Button> finishBtn = ViewsFactory::createButton("Continue", [this] {
				auto event = CustomEvents::make(EVENT_GO_NEXT_LEVEL, LevelEventData::create(m_map->getLevelId()));
				getEventDispatcher()->dispatchEvent(event);
			});
			finishBtn->setPosition(Vec2{ 0.5f * size.x, 0.3f * size.y });
			this->addChild(finishBtn, 2);

			{
				auto emitter = ParticleFireworks::create();
				emitter->setPosition(Vec2{ 0.1f * size.x, 0.1f * size.y });
				this->addChild(emitter, 1);
			}
			{
				auto emitter = ParticleFireworks::create();
				emitter->setPosition(Vec2{ 0.9f * size.x, 0.1f * size.y });
				this->addChild(emitter, 1);
			}
		}
		else
		{
			auto event = CustomEvents::make(EVENT_LOSE_ON_LEVEL, LevelEventData::create(m_map->getLevelId()));
			getEventDispatcher()->dispatchEvent(event);

			RefPtr<Label> label = ViewsFactory::createLargeLabel("Game Over\nUnfortunately, you lose...");
			label->setPosition(Vec2{ 0.5f * size.x, 0.5f * size.y });
			this->addChild(label, 1);

			RefPtr<ui::Button> finishBtn = ViewsFactory::createButton("Exit", [this] {
				auto event = CustomEvents::make(EVENT_EXIT_LEVEL, LevelEventData::create(m_map->getLevelId()));
				getEventDispatcher()->dispatchEvent(event);
			});
			finishBtn->setPosition(Vec2{ 0.65f * size.x, 0.3f * size.y });
			this->addChild(finishBtn, 2);

			RefPtr<ui::Button> retryBtn = ViewsFactory::createButton("Retry", [this] {
				auto event = CustomEvents::make(EVENT_RETRY_LEVEL, LevelEventData::create(m_map->getLevelId()));
				getEventDispatcher()->dispatchEvent(event);
			});
			retryBtn->setPosition(Vec2{ 0.35f * size.x, 0.3f * size.y });
			this->addChild(retryBtn, 2);
		}
	}
	catch (const std::exception &ex)
	{
		CCLOG("init failed: %s", ex.what());
		return false;
	}
	return true;
}
