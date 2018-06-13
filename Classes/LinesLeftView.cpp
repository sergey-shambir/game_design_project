#include "LinesLeftView.h"
#include "SpriteUtils.h"
#include "ViewsFactory.h"

using namespace cocos2d;

namespace
{
constexpr float kViewTopMargin = 4;
constexpr float kViewLeftMargin = 8;
constexpr float kItemsPadding = 8;
constexpr auto ICON_LINES = "res/icon_line.png";
} // namespace

LinesLeftView*LinesLeftView::create(unsigned linesTotal)
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
	const std::string text = std::to_string(count) + "/" + std::to_string(m_linesTotal);
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

		const std::string totalStr = std::to_string(linesTotal);
		const std::string maxLenStr = totalStr + "/" + totalStr;
		const std::string startStr = "0/" + totalStr;
		m_lineIcon = sprite_utils::createSprite(ICON_LINES);
		m_lineCount = ViewsFactory::createLargeLabel(maxLenStr);

		const Size iconSize = m_lineIcon->getContentSize();
		const Size textSize = m_lineCount->getContentSize();
		const Size contentSize = Size{
			2 * kViewLeftMargin + kItemsPadding + iconSize.width + textSize.width,
			2 * kViewTopMargin + (std::max)(iconSize.height, textSize.height)
		};
		setContentSize(contentSize);

		const Vec2 iconPos = Vec2{ kViewLeftMargin, 0.5f * contentSize.height };
		const Vec2 textPos = Vec2{ kViewLeftMargin + kItemsPadding + iconSize.width, 0.5f * contentSize.height };
		m_lineIcon->setPosition(iconPos);
		m_lineIcon->setAnchorPoint(Vec2{ 0.0f, 0.5f });
		m_lineCount->setPosition(textPos);
		m_lineCount->setAnchorPoint(Vec2{ 0.0f, 0.5f });
		m_lineCount->setString(startStr);

		this->addChild(m_lineIcon);
		this->addChild(m_lineCount);
	}
	catch (const std::exception &ex)
	{
		CCLOG("cannot init LinesLeftView: %s", ex.what());
		return false;
	}
	return true;
}
