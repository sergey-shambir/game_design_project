#include "HeadUpDisplayLayer.h"
#include "CustomEvents.h"
#include "ScoreManager.h"
#include "ViewsFactory.h"

using namespace cocos2d;

namespace
{
enum zIndex : int
{
	zIndexCommitedLines = 1,
	zIndexNewLine,
	zIndexIndicators,
	zIndexOverlay,
};

constexpr unsigned kTouchListenerPriority = 1;
const float kMinTouchSlideLength = 30.0f;
const float kLineWidth = 20.0f;
const float kTimeBarHeight = 30.0f;
const float kTimeBarTopMargin = 20.0f;

const cocos2d::Color4F kColorCommitedLine = { 0.2f, 0.2f, 0.5f, 1.0f };
const cocos2d::Color4F kColorValidLine = { 0.3f, 0.7f, 1.0f, 0.5f };
const cocos2d::Color4F kColorInvalidLine = { 1.0f, 0.2f, 0.2f, 0.5f };

float getScaledLineWidth()
{
	return Director::getInstance()->getContentScaleFactor() * kLineWidth;
}
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

	m_linesLeftView = LinesLeftView::create(m_map->getBoundaryCount());
	m_linesLeftView->setAnchorPoint(Vec2{ 0, 1 });
	m_linesLeftView->setPosition(Vec2{ 0, layerSize.height });
	this->addChild(m_linesLeftView, zIndexIndicators);

	m_scoreView = ScoreView::create();
	m_scoreView->setAnchorPoint(Vec2{ 1, 1 });
	m_scoreView->setPosition(Vec2{ layerSize.width, layerSize.height });
	this->addChild(m_scoreView, zIndexIndicators);

	const Rect linesLeftBox = m_linesLeftView->getBoundingBox();
	const Rect scoreBox = m_scoreView->getBoundingBox();
	const Size timeBarSize = { scoreBox.getMinX() - linesLeftBox.getMaxX(), kTimeBarHeight };

	m_timeLeftView = TimeLeftView::create(timeBarSize);
	m_timeLeftView->setPosition(Vec2{ linesLeftBox.getMaxX(), layerSize.height - kTimeBarTopMargin });
	m_timeLeftView->setAnchorPoint(Vec2{ 0, 1 });
	m_timeLeftView->setEstimatedTime(m_map->getEstimatedSpentSeconds());
	this->addChild(m_timeLeftView, zIndexIndicators);

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
	m_tempLineNode->setLineWidth(getScaledLineWidth());
	addChild(m_tempLineNode, zIndexNewLine);

	m_commitedLinesNode = DrawNode::create();
	m_commitedLinesNode->setContentSize(layerSize);
	m_commitedLinesNode->setLineWidth(getScaledLineWidth());
	addChild(m_commitedLinesNode, zIndexCommitedLines);

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
		addChild(m_gameIntroNode, zIndexOverlay);
	}
	else
	{
		startLevel();
	}

#if 0
	m_debugNode = DrawNode::create();
	m_debugNode->setContentSize(layerSize);
	addChild(m_debugNode, zIndexOverlay);
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
	m_scoreView->setScore(scoreManager.getScore());
	m_timeLeftView->setSecondsLeft(m_secondsLeft);
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
	m_tempLineNode->setLineWidth(getScaledLineWidth());
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
		redrawBoundaries();
		ScoreManager::getInstance().updateAfterLinePut();
		if (m_boundaries.size() >= m_map->getBoundaryCount())
		{
			checkWinLose();
		}
	}
}

void HeadUpDisplayLayer::cancelLastBoundary()
{
	m_boundaries.pop_back();
	redrawBoundaries();
	ScoreManager::getInstance().updateAfterLineCanceled();
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
	addChild(m_gameOverNode, zIndexOverlay);
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
		m_timeLeftView->scheduleUpdate();
	}
}

void HeadUpDisplayLayer::redrawBoundaries()
{
	m_commitedLinesNode->clear();
	m_commitedLinesNode->setLineWidth(getScaledLineWidth());
	for (const Line &line : m_boundaries)
	{
		Line visibleLine = line.expandAB(Rect{ Vec2{ 0, 0 }, getContentSize() });
		m_commitedLinesNode->drawLine(visibleLine.vertexA, visibleLine.vertexB, kColorCommitedLine);
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
