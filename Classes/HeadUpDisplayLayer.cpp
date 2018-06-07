#include "HeadUpDisplayLayer.h"
#include "CustomEvents.h"
#include "ScoreManager.h"
#include "ViewsFactory.h"

using namespace cocos2d;

namespace
{
constexpr unsigned kTouchListenerPriority = 1;
const float kMinTouchSlideLength = 30.0f;
const float kLineWidth = 20.f;

const cocos2d::Color4F kColorCommitedLine = { 0.2f, 0.2f, 0.5f, 1.0f };
const cocos2d::Color4F kColorValidLine = { 0.3f, 0.7f, 1.0f, 0.5f };
const cocos2d::Color4F kColorInvalidLine = { 1.0f, 0.2f, 0.2f, 0.5f };
} // namespace

HeadUpDisplayLayer *HeadUpDisplayLayer::create(const cocos2d::Size &layerSize, IGameLevelMap &map)
{
	RefPtr<HeadUpDisplayLayer> layer = new (std::nothrow) HeadUpDisplayLayer;
	layer->initWithMap(layerSize, map);
	layer->autorelease();

	return layer;
}

bool HeadUpDisplayLayer::isGameFinished() const
{
	return (m_status != GameStatus::Playing) && (m_status != GameStatus::Intro);
}

unsigned HeadUpDisplayLayer::getLinesSpent() const
{
	return static_cast<unsigned>(m_boundaries.size());
}

float HeadUpDisplayLayer::getSecondsLeft() const
{
	return m_secondsLeft;
}

void HeadUpDisplayLayer::initWithMap(const cocos2d::Size &layerSize, IGameLevelMap &map)
{
	Node::init();
	Node::setContentSize(layerSize);
	scheduleUpdate();
	m_map = &map;
	m_secondsLeft = m_map->getEstimatedSpentSeconds();

	m_linesLeftView = LinesLeftView::create();
	m_linesLeftView->setPosition(Vec2{ 0, layerSize.height });
	this->addChild(m_linesLeftView);

	m_timeScoreView = TimeScoreView::create();
	m_timeScoreView->setPosition(Vec2{ layerSize.width, layerSize.height });
	m_timeScoreView->setEstimatedTime(m_map->getEstimatedSpentSeconds());
	this->addChild(m_timeScoreView);

	m_touchListener = EventListenerTouchOneByOne::create();
	m_touchListener->onTouchBegan = [this](Touch *touch, Event *event) {
		(void)touch;
		(void)event;
		updateBoundary(touch);
		return true;
	};
	m_touchListener->onTouchMoved = [this](Touch *touch, Event *event) {
		(void)event;
		updateBoundary(touch);
	};
	m_touchListener->onTouchEnded = [=](Touch *touch, Event *event) {
		(void)event;
		commitBoundary(touch);
		m_tempLineNode->clear();
	};
	m_touchListener->onTouchCancelled = [=](Touch *touch, Event *event) {
		(void)touch;
		(void)event;
		m_tempLineNode->clear();
	};
	m_touchListener->setSwallowTouches(true);

	m_tempLineNode = DrawNode::create();
	m_tempLineNode->setContentSize(layerSize);
	m_tempLineNode->setLineWidth(kLineWidth);
	addChild(m_tempLineNode, 3);

	m_commitedLinesNode = DrawNode::create();
	m_commitedLinesNode->setContentSize(layerSize);
	m_commitedLinesNode->setLineWidth(kLineWidth);
	addChild(m_commitedLinesNode, 2);

	if (!ScoreManager::getInstance().didShowIntro())
	{
		m_startListener = EventListenerCustom::create(EVENT_START_LEVEL, [this](EventCustom *event) {
			if (LevelEventData *data = CustomEvents::get<LevelEventData>(event))
			{
				if (data->getLevelId() == m_map->getLevelId())
				{
					ScoreManager::getInstance().updateAfterIntroShown();
					getEventDispatcher()->addEventListenerWithFixedPriority(m_touchListener, kTouchListenerPriority);
					startLevel();
				}
			}
		});

		m_gameIntroNode = GameIntroLayer::create(getContentSize(), m_map->getLevelId());
		addChild(m_gameIntroNode, 5);
	}
	else
	{
		startLevel();
	}

#if 0
	m_debugNode = DrawNode::create();
	m_debugNode->setContentSize(layerSize);
	addChild(m_debugNode, 1);
	for (const Rect &animal : m_map->getAnimalsRects())
	{
		m_debugNode->drawRect(animal.origin, animal.origin + size2vec(animal.size), kColorValidLine);
	}
	for (const Rect &plant : m_map->getPlantsRects())
	{
		m_debugNode->drawRect(plant.origin, plant.origin + size2vec(plant.size), kColorInvalidLine);
	}
#endif
}

void HeadUpDisplayLayer::update(float delta)
{
	if (m_status == GameStatus::Playing)
	{
		m_secondsLeft = (std::max)(0.0f, m_secondsLeft - delta);
		if (m_secondsLeft == 0.0f)
		{
			m_status = GameStatus::Lose;
			finishRound();
		}
	}
	ScoreManager &scoreManager = ScoreManager::getInstance();
	m_linesLeftView->setLinesLeft(scoreManager.getLinesLeft());
	m_timeScoreView->setScore(scoreManager.getScore());
	m_timeScoreView->setSecondsLeft(static_cast<unsigned>(std::round(m_secondsLeft)));
	Node::update(delta);
}

void HeadUpDisplayLayer::onEnter()
{
	Node::onEnter();
	if (m_status == GameStatus::Playing)
	{
		getEventDispatcher()->addEventListenerWithFixedPriority(m_touchListener, kTouchListenerPriority);
	}
	if (m_startListener)
	{
		getEventDispatcher()->addEventListenerWithFixedPriority(m_startListener, kTouchListenerPriority);
	}
}

void HeadUpDisplayLayer::onExit()
{
	getEventDispatcher()->removeEventListener(m_touchListener);
	if (m_startListener)
	{
		getEventDispatcher()->removeEventListener(m_startListener);
	}
	Node::onExit();
}

void HeadUpDisplayLayer::updateBoundary(Touch *touch)
{
	m_nextBoundary = { touch->getStartLocation(), touch->getLocation() };
	m_isNextBoundaryValid = isBoundaryValid(m_nextBoundary);

	m_tempLineNode->clear();
	m_tempLineNode->setLineWidth(kLineWidth);
	if (m_nextBoundary.getDistanceAB() >= kMinTouchSlideLength)
	{
		Color4F color = m_isNextBoundaryValid ? kColorValidLine : kColorInvalidLine;
		Line visibleLine = m_nextBoundary.expandAB(Rect{ Vec2{ 0, 0 }, getContentSize() });
		m_tempLineNode->drawLine(visibleLine.vertexA, visibleLine.vertexB, color);
	}
}

void HeadUpDisplayLayer::commitBoundary(Touch *touch)
{
	updateBoundary(touch);
	if (m_isNextBoundaryValid)
	{
		m_boundaries.push_back(m_nextBoundary);
		Line visibleLine = m_nextBoundary.expandAB(Rect{ Vec2{ 0, 0 }, getContentSize() });
		m_commitedLinesNode->drawLine(visibleLine.vertexA, visibleLine.vertexB, kColorCommitedLine);

		ScoreManager::getInstance().updateAfterLinePut();
		if (m_boundaries.size() >= m_map->getBoundaryCount())
		{
			checkWinLose();
		}
	}
}

void HeadUpDisplayLayer::checkWinLose()
{
	if (doePlayerWin())
	{
		m_status = GameStatus::Win;
		finishRound();
	}
	else if (ScoreManager::getInstance().getLinesLeft() == 0)
	{
		m_status = GameStatus::Lose;
		finishRound();
	}
}

void HeadUpDisplayLayer::finishRound()
{
	assert(m_map);

	getEventDispatcher()->removeEventListener(m_touchListener);
	m_gameOverNode = GameOverLayer::create(getContentSize(), *m_map, m_status);
	addChild(m_gameOverNode, 5);
}

void HeadUpDisplayLayer::startLevel()
{
	assert(m_status == GameStatus::Intro);
	if (m_status == GameStatus::Intro)
	{
		if (m_gameIntroNode)
		{
			m_gameIntroNode->removeAllChildrenWithCleanup(true);
			m_gameIntroNode->removeFromParentAndCleanup(true);
		}
		if (m_startListener)
		{
			getEventDispatcher()->removeEventListener(m_startListener);
		}
		m_status = GameStatus::Playing;
		m_timeScoreView->scheduleUpdate();
	}
}

bool HeadUpDisplayLayer::isBoundaryValid(const Line &boundary)
{
	if (boundary.getDistanceAB() < kMinTouchSlideLength)
	{
		return false;
	}

	assert(m_map);
	auto obstacles = m_map->getAllObstacles();
	auto it = std::find_if(obstacles.begin(), obstacles.end(), [&](const Rect &rect) {
		return boundary.intersectsRect(rect);
	});

	return (it == obstacles.end());
}

bool HeadUpDisplayLayer::doePlayerWin() const
{
	assert(m_map);
	auto animalsRects = m_map->getAnimalsRects();
	auto plantsRects = m_map->getPlantsRects();
	for (const Rect &animal : animalsRects)
	{
		for (const Rect &plant : plantsRects)
		{
			auto it = std::find_if(m_boundaries.begin(), m_boundaries.end(), [&](const Line &line) {
				return line.isPointOnLeft(plant.origin) != line.isPointOnLeft(animal.origin);
			});
			if (it == m_boundaries.end())
			{
				// There is animal-plant pair without boundary between.
				return false;
			}
		}
	}

	return true;
}
