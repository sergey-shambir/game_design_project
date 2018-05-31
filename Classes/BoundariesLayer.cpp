#include "BoundariesLayer.h"

using namespace cocos2d;

namespace
{
constexpr unsigned kTouchListenerPriority = 1;
const float kMinTouchSlideLength = 60.0f;
const float kLineWidth = 20.f;

const cocos2d::Color4F kColorCommitedLine = { 0.2f, 0.2f, 0.5f, 1.0f };
const cocos2d::Color4F kColorValidLine = { 0.3f, 0.7f, 1.0f, 0.5f };
const cocos2d::Color4F kColorInvalidLine = { 1.0f, 0.2f, 0.2f, 0.5f };
} // namespace

BoundariesLayer *BoundariesLayer::create(const cocos2d::Size &layerSize, IGameLevelMap &map)
{
	RefPtr<BoundariesLayer> layer = new (std::nothrow) BoundariesLayer;
	layer->initWithMap(layerSize, map);
	layer->autorelease();

	return layer;
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

	m_tempLineNode = DrawNode::create();
	m_tempLineNode->setContentSize(layerSize);
	m_tempLineNode->setLineWidth(kLineWidth);
	addChild(m_tempLineNode, 3);

	m_commitedLinesNode = DrawNode::create();
	m_commitedLinesNode->setContentSize(layerSize);
	m_commitedLinesNode->setLineWidth(kLineWidth);
	addChild(m_commitedLinesNode, 2);

	m_debugNode = DrawNode::create();
	m_debugNode->setContentSize(layerSize);
	addChild(m_debugNode, 1);

	for (const Rect& animal : m_map->getAnimalsRects())
	{
		m_debugNode->drawRect(animal.origin, animal.origin + Vec2(animal.size), kColorValidLine);
	}
	for (const Rect& plant : m_map->getPlantsRects())
	{
		m_debugNode->drawRect(plant.origin, plant.origin + Vec2(plant.size), kColorInvalidLine);
	}
}

void BoundariesLayer::onEnter()
{
	getEventDispatcher()->addEventListenerWithFixedPriority(m_touchListener, kTouchListenerPriority);
}

void BoundariesLayer::onExit()
{
	getEventDispatcher()->removeEventListener(m_touchListener);
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

		if (m_boundaries.size() >= m_map->getBoundaryCount())
		{
			checkWinLose();
		}
	}
}

void BoundariesLayer::checkWinLose()
{
	m_status = doePlayerWin() ? GameStatus::Win : GameStatus::Lose;
	// TODO: add "Finish" button
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
