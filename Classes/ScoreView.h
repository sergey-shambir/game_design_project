#pragma once

#include "cocos2dgame.h"

class ScoreView : public cocos2d::DrawNode
{
public:
	CREATE_FUNC(ScoreView);

	void setScore(unsigned value);
	bool init() override;

private:
	unsigned m_score = 0;
	cocos2d::RefPtr<cocos2d::Sprite> m_indicatorBg;
	cocos2d::RefPtr<cocos2d::Label> m_scoreLabel;
};
