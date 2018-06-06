#pragma once

#include "cocos2dgame.h"

class LinesLeftView : public cocos2d::Node
{
public:
    CREATE_FUNC(LinesLeftView);

    void setLinesLeft(unsigned count);

private:
    std::vector<cocos2d::Sprite*> m_lines;
};
