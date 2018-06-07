#include "GameLevelScene.h"
#include "CustomEvents.h"

using namespace cocos2d;
using namespace std::literals;

Scene *GameLevelScene::createScene(const std::string &levelId)
{
	return GameLevelScene::create(levelId);
}

GameLevelScene *GameLevelScene::create(const std::string &levelId)
{
	RefPtr<GameLevelScene> scene = new (std::nothrow) GameLevelScene;
	scene->m_levelId = levelId;
	if (scene->init())
	{
		scene->autorelease();
		return scene;
	}
	return nullptr;
}

void GameLevelScene::tryInit()
{
	scheduleUpdate();
	preloadResources();

	Size mapSize = getContentSize();
	m_map = GameLevelMap::create(mapSize, m_levelId);
	this->addChild(m_map, 1);
	m_hud = HeadUpDisplayLayer::create(mapSize, *m_map);
	this->addChild(m_hud, 2);

	m_listeners.emplace_back(EventListenerCustom::create(EVENT_WIN_ON_LEVEL, [this](EventCustom *event) {
		if (LevelEventData *data = CustomEvents::get<LevelEventData>(event))
		{
			if (data->getLevelId() == m_levelId)
			{
				reportWin();
			}
		}
	}));
	m_listeners.emplace_back(EventListenerCustom::create(EVENT_GO_NEXT_LEVEL, [this](EventCustom *event) {
		if (LevelEventData *data = CustomEvents::get<LevelEventData>(event))
		{
			if (data->getLevelId() == m_levelId)
			{
				removeListeners();
				switchNextScene();
			}
		}
	}));
	m_listeners.emplace_back(EventListenerCustom::create(EVENT_EXIT_LEVEL, [this](EventCustom *event) {
		if (LevelEventData *data = CustomEvents::get<LevelEventData>(event))
		{
			if (data->getLevelId() == m_levelId)
			{
				removeListeners();
				switchWelcomeScene();
			}
		}
	}));

	m_listeners.emplace_back(EventListenerCustom::create(EVENT_RETRY_LEVEL, [this](EventCustom *event) {
		if (LevelEventData *data = CustomEvents::get<LevelEventData>(event))
		{
			if (data->getLevelId() == m_levelId)
			{
				auto *scene = GameLevelScene::createScene(m_levelId);
				Director::getInstance()->replaceScene(TransitionFade::create(1.0f, scene));
			}
		}
	}));

	ScoreManager::getInstance().updateBeforeRoundStart(getRoundConditions());
}

void GameLevelScene::preloadResources()
{
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("res/objects.plist");
}

void GameLevelScene::removeListeners()
{
	for (auto&& listener : m_listeners)
	{
		getEventDispatcher()->removeEventListener(listener);
	}
}

void GameLevelScene::reportWin()
{
	RoundResults results;
	results.linesSpent = m_hud->getLinesSpent();
	results.secondsSpent = m_secondsLeft;
	ScoreManager::getInstance().updateAfterRoundWin(getRoundConditions(), results);
}

void GameLevelScene::switchNextScene()
{
	auto runLevel = [](const std::string &levelId) {
		auto *scene = GameLevelScene::createScene(levelId);
		Director::getInstance()->replaceScene(TransitionFade::create(1.0f, scene));
	};

	if (m_levelId == GAME_LEVEL1_ID)
	{
		runLevel(GAME_LEVEL2_ID);
	}
	else if (m_levelId == GAME_LEVEL2_ID)
	{
		runLevel(GAME_LEVEL3_ID);
	}
	else
	{
		switchWelcomeScene();
	}
}

void GameLevelScene::switchWelcomeScene()
{
	Director::getInstance()->popScene();
}

RoundConditions GameLevelScene::getRoundConditions() const
{
	RoundConditions conditions;
	conditions.linesMin = m_map->getBoundaryCount();
	conditions.estimatedSeconds = m_map->getEstimatedSpentSeconds();

	return conditions;
}

void GameLevelScene::update(float delta)
{
	m_secondsLeft += delta;
	AbstractScene::update(delta);
}

void GameLevelScene::onEnter()
{
	AbstractScene::onEnter();
	for (auto&& listener : m_listeners)
	{
		getEventDispatcher()->addEventListenerWithFixedPriority(listener, 1);
	}
}

void GameLevelScene::onExit()
{
	AbstractScene::onExit();
	removeListeners();
}
