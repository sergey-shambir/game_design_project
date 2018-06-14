#include "ScoreView.h"
#include "SpriteUtils.h"
#include "ViewsFactory.h"

using namespace cocos2d;

namespace
{
constexpr float kViewTopMargin = 4;
constexpr float kViewLeftMargin = 4;
constexpr auto kIndicatorBgImage = "res/score-indicator-bg.png";
const Vec2 kLabelPosition = Vec2{ 100, 30 };
} // namespace

void ScoreView::setScore(unsigned value)
{
	if (m_score != value)
	{
		m_score = value;
		m_scoreLabel->setString(std::to_string(value));
	}
}

bool ScoreView::init()
{
	if (!Node::init())
	{
		return false;
	}
	try
	{
		const std::string maxVisibleScore = "999";
		const std::string initialScore = "0";
		m_indicatorBg = sprite_utils::createSprite(kIndicatorBgImage);
		m_scoreLabel = ViewsFactory::createIndicatorLabel(maxVisibleScore);

		const Size bgSize = m_indicatorBg->getContentSize();
		const Size contentSize = { 2 * kViewLeftMargin + bgSize.width, 2 * kViewTopMargin + bgSize.height };
		setContentSize(contentSize);

		m_indicatorBg->setPosition(Vec2{ kViewLeftMargin, kViewTopMargin });
		m_indicatorBg->setAnchorPoint(Vec2{ 0, 0 });
		this->addChild(m_indicatorBg);

		m_scoreLabel->setPosition(kLabelPosition + Vec2{ kViewLeftMargin, kViewTopMargin });
		m_scoreLabel->setAnchorPoint(Vec2{ 0.5f, 0.5f });
		m_scoreLabel->setString(initialScore);
		this->addChild(m_scoreLabel);
	}
	catch (const std::exception &ex)
	{
		CCLOG("cannot init LinesLeftView: %s", ex.what());
		return false;
	}
	return true;
}
