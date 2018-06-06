#include "LinesLeftView.h"
#include "SpriteUtils.h"

using namespace cocos2d;

namespace
{
constexpr auto ICON_STAR = "res/icon_star.png";
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
            auto sprite = sprite_utils::createSprite(ICON_STAR);
            const Size size = sprite->getContentSize();
            sprite->setPosition(Vec2{ i * size.width, 0 });
			sprite->setOpacity(0);
			sprite->runAction(FadeIn::create(kFadeTimeSeconds));
            addChild(sprite);
            m_lines.emplace_back(sprite);
        }
    }
}
