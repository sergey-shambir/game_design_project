#pragma once

#include "cocos2dgame.h"

class TimeScoreView : public cocos2d::DrawNode
{
public:
	CREATE_FUNC(TimeScoreView);

	void setEstimatedTime(float seconds);
	void setScore(unsigned value);
	void setSecondsLeft(unsigned value);

	bool init() override;

private:
	std::string formatScore() const;
	std::string formatSecondsPassed() const;
	void updateBackground();

	unsigned m_score = 0;
	unsigned m_secondsLeft = 0;
	cocos2d::RefPtr<cocos2d::Label> m_scoreLabel;
	cocos2d::RefPtr<cocos2d::Label> m_timeLeftLabel;
};
