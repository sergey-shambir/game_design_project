#pragma once

#include "cocos2dgame.h"

class TimeLeftView : public cocos2d::DrawNode
{
public:
	static TimeLeftView* create(const cocos2d::Size& contentSize);

	void setEstimatedTime(float seconds);
	void setSecondsLeft(float value);

	bool initWithSize(const cocos2d::Size& contentSize);

private:
	std::string formatSecondsPassed() const;
	void redraw();

	float m_estimatedTime = 0;
	float m_secondsLeft = 0;
};
