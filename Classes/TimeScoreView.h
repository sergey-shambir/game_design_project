#pragma once

#include "cocos2dgame.h"

class TimeScoreView : public cocos2d::Node
{
public:
	CREATE_FUNC(TimeScoreView);

	void setEstimatedTime(float seconds);
	void setScore(unsigned value);

	bool init() override;
	void update(float delta) override;

private:
	std::string formatScore() const;
	std::string formatSecondsLeft() const;
	void setSecondsLeft(unsigned value);

	unsigned m_score = 0;
	float m_timeLeft = 0;
	unsigned m_secondsLeft = 0;
	cocos2d::RefPtr<cocos2d::Label> m_scoreLabel;
	cocos2d::RefPtr<cocos2d::Label> m_timeLeftLabel;
};
