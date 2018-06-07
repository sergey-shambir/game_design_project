#include "TimeScoreView.h"
#include "ViewsFactory.h"

using namespace cocos2d;

namespace
{
constexpr float kViewBorder = 3.0f;
constexpr float kViewTopMargin = 8.0f;
constexpr float kViewRightMargin = 8.0f;
constexpr float kItemsPadding = 8.0f;

const Color4F kViewBackgroundColor = { 0.2f, 0.2f, 0.2f, 0.6f };
} // namespace

void TimeScoreView::setEstimatedTime(float seconds)
{
	(void)seconds;
}

void TimeScoreView::setScore(unsigned value)
{
	if (m_score != value)
	{
		m_score = value;
		m_scoreLabel->setString(formatScore());
		updateBackground();
	}
}

void TimeScoreView::setSecondsLeft(unsigned value)
{
	if (m_secondsLeft != value)
	{
		m_secondsLeft = value;
		m_timeLeftLabel->setString(formatSecondsPassed());
		updateBackground();
	}
}

bool TimeScoreView::init()
{
	if (!DrawNode::init())
	{
		return false;
	}
	try
	{
		m_scoreLabel = ViewsFactory::createLargeLabel(formatScore());
		m_timeLeftLabel = ViewsFactory::createLargeLabel(formatSecondsPassed());

		const Vec2 timeLeftPos = Vec2{ -kViewRightMargin, -kViewTopMargin };
		const Size timeLeftSize = m_timeLeftLabel->getContentSize();
		const Vec2 scorePos = timeLeftPos - Vec2{ kItemsPadding + timeLeftSize.width, 0 };

		m_timeLeftLabel->setAnchorPoint(Vec2{ 1, 1 });
		m_timeLeftLabel->setPosition(timeLeftPos);
		addChild(m_timeLeftLabel);

		m_scoreLabel->setAnchorPoint(Vec2{ 1, 1 });
		m_scoreLabel->setPosition(scorePos);
		addChild(m_scoreLabel);

		updateBackground();
	}
	catch (const std::exception &ex)
	{
		CCLOG("init failed: %s", ex.what());
		return false;
	}
	return true;
}

std::string TimeScoreView::formatScore() const
{
	constexpr auto kPrefix = "Score: ";
	return kPrefix + std::to_string(m_score);
}

std::string TimeScoreView::formatSecondsPassed() const
{
	constexpr auto kPrefix = "Time: ";
	unsigned seconds = m_secondsLeft % 60;
	unsigned minutes = m_secondsLeft / 60;
	char timeStr[64] = { 0 };
	snprintf(timeStr, sizeof(timeStr) / sizeof(char), "%02d:%02d", minutes, seconds);

	return std::string(kPrefix) + std::string(timeStr);
}

void TimeScoreView::updateBackground()
{
	Rect bgRect = m_timeLeftLabel->getBoundingBox().unionWithRect(m_scoreLabel->getBoundingBox());
	bgRect.size.width += 2 * kViewBorder;
	bgRect.size.height += 2 * kViewBorder;
	bgRect.origin.x -= kViewBorder;
	bgRect.origin.y -= kViewBorder;
	clear();
	drawSolidRect(Vec2{ bgRect.getMinX(), bgRect.getMinY() }, Vec2{ bgRect.getMaxX(), bgRect.getMaxY() }, kViewBackgroundColor);
}
