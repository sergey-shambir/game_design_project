#include "BoundariesLayer.h"
#include "CustomEvents.h"
#include "ViewsFactory.h"
#include "ScoreManager.h"

using namespace cocos2d;

namespace
{
constexpr unsigned kTouchListenerPriority = 1;
const float kMinTouchSlideLength = 60.0f;
const float kLineWidth = 20.f;

const cocos2d::Color4F kColorCommitedLine = { 0.2f, 0.2f, 0.5f, 1.0f };
const cocos2d::Color4F kColorValidLine = { 0.3f, 0.7f, 1.0f, 0.5f };
const cocos2d::Color4F kColorInvalidLine = { 1.0f, 0.2f, 0.2f, 0.5f };
const cocos2d::Color4F kGameOverSplashColor = { 0.0f, 0.0f, 0.0f, 0.6f };

Vec2 size2vec(const Size& size)
{
	return Vec2{ size.width, size.height };
}
} // namespace

BoundariesLayer *BoundariesLayer::create(const cocos2d::Size &layerSize, IGameLevelMap &map)
{
	RefPtr<BoundariesLayer> layer = new (std::nothrow) BoundariesLayer;
	layer->initWithMap(layerSize, map);
	layer->autorelease();

	return layer;
}

bool BoundariesLayer::isGameFinished() const
{
	return (m_status != GameStatus::Playing);
}

unsigned BoundariesLayer::getLinesSpent() const
{
	return static_cast<unsigned>(m_boundaries.size());
}

void BoundariesLayer::initWithMap(const cocos2d::Size &layerSize, IGameLevelMap &map)
{
	Node::init();
	Node::setContentSize(layerSize);

	m_map = &map;
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

	m_gameOverNode = DrawNode::create();
	m_gameOverNode->setContentSize(getContentSize());
	addChild(m_gameOverNode, 5);

	m_tempLineNode = DrawNode::create();
	m_tempLineNode->setContentSize(layerSize);
	m_tempLineNode->setLineWidth(kLineWidth);
	addChild(m_tempLineNode, 3);

	m_commitedLinesNode = DrawNode::create();
	m_commitedLinesNode->setContentSize(layerSize);
	m_commitedLinesNode->setLineWidth(kLineWidth);
	addChild(m_commitedLinesNode, 2);

#if 0
	m_debugNode = DrawNode::create();
	m_debugNode->setContentSize(layerSize);
	addChild(m_debugNode, 1);
APP_PLATFORM := android-16
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

void BoundariesLayer::onEnter()
{
	Node::onEnter();
	if (m_status == GameStatus::Playing)
	{
		getEventDispatcher()->addEventListenerWithFixedPriority(m_touchListener, kTouchListenerPriority);
	}
}

void BoundariesLayer::onExit()
{
	getEventDispatcher()->removeEventListener(m_touchListener);
	Node::onExit();
}

void BoundariesLayer::updateBoundary(Touch *touch)
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

void BoundariesLayer::commitBoundary(Touch *touch)
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

void BoundariesLayer::checkWinLose()
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

void BoundariesLayer::finishRound()
{
	getEventDispatcher()->removeEventListener(m_touchListener);

	const Vec2 size{ size2vec(getContentSize()) };
	m_gameOverNode->drawSolidRect(Vec2{ 0, 0 }, size, kGameOverSplashColor);

	if (m_status == GameStatus::Win)
	{
		RefPtr<Label> label = ViewsFactory::createLargeLabel("Game Over\nCongratulations, you won!");
		label->setPosition(Vec2{ 0.5f * size.x, 0.7f * size.y });
		m_gameOverNode->addChild(label, 1);

		RefPtr<ui::Button> finishBtn = ViewsFactory::createButton("Continue", [this] {
			auto event = CustomEvents::make(EVENT_WIN_ON_LEVEL, LevelEventData::create(m_map->getLevelId()));
			getEventDispatcher()->dispatchEvent(event);
		});
		finishBtn->setPosition(Vec2{ 0.5f * size.x, 0.3f * size.y });
		m_gameOverNode->addChild(finishBtn, 2);

		{
			auto emitter = ParticleFireworks::create();
			emitter->setPosition(Vec2{ 0.1f * size.x, 0.1f * size.y });
			m_gameOverNode->addChild(emitter, 1);
		}
		{
			auto emitter = ParticleFireworks::create();
			emitter->setPosition(Vec2{ 0.9f * size.x, 0.1f * size.y });
			m_gameOverNode->addChild(emitter, 1);
		}
	}
	else
	{
		RefPtr<Label> label = ViewsFactory::createLargeLabel("Game Over\nUnfortunately, you lose...");
		label->setPosition(Vec2{ 0.5f * size.x, 0.5f * size.y });
		m_gameOverNode->addChild(label, 1);

		RefPtr<ui::Button> finishBtn = ViewsFactory::createButton("Exit", [this] {
			auto event = CustomEvents::make(EVENT_LOSE_ON_LEVEL, LevelEventData::create(m_map->getLevelId()));
			getEventDispatcher()->dispatchEvent(event);
		});
		finishBtn->setPosition(Vec2{ 0.65f * size.x, 0.3f * size.y });
		m_gameOverNode->addChild(finishBtn, 2);

		RefPtr<ui::Button> retryBtn = ViewsFactory::createButton("Retry", [this] {
			auto event = CustomEvents::make(EVENT_RETRY_LEVEL, LevelEventData::create(m_map->getLevelId()));
			getEventDispatcher()->dispatchEvent(event);
		});
		retryBtn->setPosition(Vec2{ 0.35f * size.x, 0.3f * size.y });
		m_gameOverNode->addChild(retryBtn, 2);
	}
}

bool BoundariesLayer::isBoundaryValid(const Line &boundary)
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

bool BoundariesLayer::doePlayerWin() const
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
