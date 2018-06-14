#include "LinesLeftView.h"
#include "SpriteUtils.h"
#include "ViewsFactory.h"

using namespace cocos2d;

namespace
{
constexpr float kViewTopMargin = 4;
constexpr float kViewLeftMargin = 4;
constexpr auto kIndicatorBgImage = "res/boundary-indicator-bg.png";
const Vec2 kLabelPosition = Vec2{ 100, 30 };
} // namespace

LinesLeftView *LinesLeftView::create(unsigned linesTotal)
{
	RefPtr<LinesLeftView> self = new (std::nothrow) LinesLeftView;
	if (self && self->initWithLines(linesTotal))
	{
		self->autorelease();
		return self;
	}
	return nullptr;
}

void LinesLeftView::setLinesLeft(unsigned count)
{
	const std::string text = "x" + std::to_string(count);
	m_lineCount->setString(text);
}

bool LinesLeftView::initWithLines(unsigned linesTotal)
{
	if (!Node::init())
	{
		return false;
	}
	try
	{
		m_linesTotal = linesTotal;

		const std::string text = "x" + std::to_string(linesTotal);
		m_indicatorBg = sprite_utils::createSprite(kIndicatorBgImage);
		m_lineCount = ViewsFactory::createIndicatorLabel(text);

		const Size bgSize = m_indicatorBg->getContentSize();
		const Size contentSize = { 2 * kViewLeftMargin + bgSize.width, 2 * kViewTopMargin + bgSize.height };
		setContentSize(bgSize);

		m_indicatorBg->setPosition(Vec2{ kViewLeftMargin, -kViewTopMargin });
		m_indicatorBg->setAnchorPoint(Vec2{ 0, 0 });
		this->addChild(m_indicatorBg);

		m_lineCount->setPosition(kLabelPosition + Vec2{ kViewLeftMargin, -kViewTopMargin });
		m_lineCount->setAnchorPoint(Vec2{ 0.5f, 0.5f });
		this->addChild(m_lineCount);
	}
	catch (const std::exception &ex)
	{
		CCLOG("cannot init LinesLeftView: %s", ex.what());
		return false;
	}
	return true;
}
