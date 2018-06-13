#pragma once

#include "cocos2dgame.h"

class LinesLeftView : public cocos2d::Node
{
public:
	static LinesLeftView *create(unsigned linesTotal);

    void setLinesLeft(unsigned count);

private:
	bool initWithLines(unsigned linesTotal);

	unsigned m_linesTotal = 0;
	cocos2d::RefPtr<cocos2d::Sprite> m_lineIcon;
	cocos2d::RefPtr<cocos2d::Label> m_lineCount;
};
