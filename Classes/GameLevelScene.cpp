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
	m_hud = BoundariesLayer::create(mapSize, *m_map);
	this->addChild(m_hud, 2);

	m_winListener = EventListenerCustom::create(EVENT_WIN_ON_LEVEL, [this](EventCustom *event) {
		if (LevelEventData *data = CustomEvents::get<LevelEventData>(event))
		{
			if (data->getLevelId() == m_levelId)
			{
				removeListeners();
				reportWin();
				switchNextScene();
			}
		}
	});
	m_loseListener = EventListenerCustom::create(EVENT_LOSE_ON_LEVEL, [this](EventCustom *event) {
		if (LevelEventData *data = CustomEvents::get<LevelEventData>(event))
		{
			if (data->getLevelId() == m_levelId)
			{
				removeListeners();
				switchWelcomeScene();
			}
		}
	});

	m_retryListener = EventListenerCustom::create(EVENT_RETRY_LEVEL, [this](EventCustom *event) {
		if (LevelEventData *data = CustomEvents::get<LevelEventData>(event))
		{
			if (data->getLevelId() == m_levelId)
			{
				auto *scene = GameLevelScene::createScene(m_levelId);
				Director::getInstance()->replaceScene(TransitionFade::create(1.0f, scene));
			}
		}
	});

	ScoreManager::getInstance().updateBeforeRoundStart(getRoundConditions());
}

void GameLevelScene::preloadResources()
{
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("res/objects.plist");
}

void GameLevelScene::removeListeners()
{
	getEventDispatcher()->removeEventListener(m_winListener);
	getEventDispatcher()->removeEventListener(m_loseListener);
	getEventDispatcher()->removeEventListener(m_retryListener);
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
	getEventDispatcher()->addEventListenerWithFixedPriority(m_winListener, 1);
	getEventDispatcher()->addEventListenerWithFixedPriority(m_loseListener, 1);
	getEventDispatcher()->addEventListenerWithFixedPriority(m_retryListener, 1);
}

void GameLevelScene::onExit()
{
	AbstractScene::onExit();
	removeListeners();
}
