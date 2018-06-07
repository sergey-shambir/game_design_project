#include "TimeScoreView.h"
#include "ViewsFactory.h"

using namespace cocos2d;

namespace
{
constexpr float kViewTopMargin = 8.0f;
constexpr float kViewRightMargin = 8.0f;
constexpr float kItemsPadding = 8.0f;

const Color4B kLiteRedColor = { 200, 64, 64, 255 };
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
	}
}

bool TimeScoreView::init()
{
	if (!Node::init())
	{
		return false;
	}
	try
	{
		scheduleUpdate();

		m_scoreLabel = ViewsFactory::createLargeLabel(formatScore());
		m_timeLeftPassed = ViewsFactory::createLargeLabel(formatSecondsPassed());

		const Vec2 timeLeftPos = Vec2{ -kViewRightMargin, -kViewTopMargin };
		const Size timeLeftSize = m_timeLeftPassed->getContentSize();
		const Vec2 scorePos = timeLeftPos - Vec2{ kItemsPadding + timeLeftSize.width, 0 };

		m_timeLeftPassed->setAnchorPoint(Vec2{ 1, 1 });
		m_timeLeftPassed->setPosition(timeLeftPos);
		addChild(m_timeLeftPassed);

		m_scoreLabel->setAnchorPoint(Vec2{ 1, 1 });
		m_scoreLabel->setPosition(scorePos);
		addChild(m_scoreLabel);
	}
	catch (const std::exception &ex)
	{
		CCLOG("init failed: %s", ex.what());
		return false;
	}
	return true;
}

void TimeScoreView::update(float delta)
{
	m_timeLeft += delta;
	setSecondsPassed(static_cast<unsigned>(std::round(m_timeLeft)));
	Node::update(delta);
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
	snprintf(timeStr, std::size(timeStr), "%02d:%02d", minutes, seconds);

	return std::string(kPrefix) + std::string(timeStr);
}

void TimeScoreView::setSecondsPassed(unsigned value)
{
	if (m_secondsLeft != value)
	{
		m_secondsLeft = value;
		m_timeLeftPassed->setString(formatSecondsPassed());
	}
}
