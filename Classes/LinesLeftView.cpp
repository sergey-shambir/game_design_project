#include "LinesLeftView.h"
#include "SpriteUtils.h"

using namespace cocos2d;

namespace
{
constexpr float kViewTopMargin = 20;
constexpr float kViewLeftMargin = 16;
constexpr float kItemsPadding = 8;
constexpr auto ICON_LINES = "res/icon_line.png";
}

void LinesLeftView::setLinesLeft(unsigned count)
{
	constexpr float kFadeTimeSeconds = 0.5f;

    if (count < m_lines.size())
    {
        for (unsigned i = count; i < m_lines.size(); ++i)
        {
			auto* fadeAction = FadeOut::create(kFadeTimeSeconds);
			auto* removeAction = RemoveSelf::create(true);
			m_lines[i]->runAction(Sequence::create(fadeAction, removeAction, nullptr));
        }
        m_lines.erase(m_lines.begin() + count, m_lines.end());
    }
    else
    {
        for (unsigned  i = m_lines.size(); i < count; ++i)
        {
			auto sprite = sprite_utils::createSprite(ICON_LINES);
			const Size itemSize = sprite->getContentSize();
			sprite->setPosition(Vec2{ kViewLeftMargin + i * (itemSize.width + kItemsPadding), -kViewTopMargin });
			sprite->setOpacity(0);
			sprite->runAction(FadeIn::create(kFadeTimeSeconds));
            addChild(sprite);
            m_lines.emplace_back(sprite);
        }
    }
}
