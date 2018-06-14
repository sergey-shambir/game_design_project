#pragma once

#include "GameIntroLayer.h"
#include "GameOverLayer.h"
#include "GameStatus.h"
#include "IGameLevelMap.h"
#include "Line.h"
#include "LinesLeftView.h"
#include "TimeLeftView.h"
#include "ScoreView.h"

class HeadUpDisplayLayer : public cocos2d::Node
{
public:
	static HeadUpDisplayLayer *create(const cocos2d::Size &layerSize, IGameLevelMap &map);

	bool isGameFinished() const;
	unsigned getLinesSpent() const;
	float getSecondsLeft() const;

private:
	void initWithMap(const cocos2d::Size &layerSize, IGameLevelMap &map);
	void update(float delta) override;
	void onEnter() override;
	void onExit() override;
	void updateBoundary(cocos2d::Touch *touch);
	void commitBoundary(cocos2d::Touch *touch);
	void cancelLastBoundary();
	void checkWinLose();
	void finishRound();
	void startLevel();
	void redrawBoundaries();

	bool isBoundaryValid(const Line &boundary);
	bool doePlayerWin() const;

	cocos2d::RefPtr<cocos2d::EventListenerTouchOneByOne> m_touchListener;
	cocos2d::RefPtr<cocos2d::EventListenerCustom> m_startListener;
	cocos2d::RefPtr<cocos2d::DrawNode> m_tempLineNode;
	cocos2d::RefPtr<cocos2d::DrawNode> m_commitedLinesNode;
	cocos2d::RefPtr<GameOverLayer> m_gameOverNode;
	cocos2d::RefPtr<GameIntroLayer> m_gameIntroNode;
	cocos2d::RefPtr<cocos2d::DrawNode> m_debugNode;
	cocos2d::RefPtr<LinesLeftView> m_linesLeftView;
	cocos2d::RefPtr<TimeLeftView> m_timeLeftView;
	cocos2d::RefPtr<ScoreView> m_scoreView;

	Line m_nextBoundary;
	bool m_isNextBoundaryValid = false;
	std::vector<Line> m_boundaries;
	IGameLevelMap *m_map = nullptr;
	GameStatus m_status = GameStatus::Intro;
	float m_secondsLeft = 0;
};
