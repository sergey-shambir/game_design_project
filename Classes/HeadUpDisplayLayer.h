#pragma once

#include "GameStatus.h"
#include "IGameLevelMap.h"
#include "Line.h"
#include "LinesLeftView.h"
#include "TimeScoreView.h"
#include "GameOverLayer.h"

class HeadUpDisplayLayer : public cocos2d::Node
{
public:
	static HeadUpDisplayLayer *create(const cocos2d::Size &layerSize, IGameLevelMap &map);

	bool isGameFinished() const;
	unsigned getLinesSpent() const;

private:
	void initWithMap(const cocos2d::Size &layerSize, IGameLevelMap &map);
	void update(float delta) override;
	void onEnter() override;
	void onExit() override;
	void updateBoundary(cocos2d::Touch *touch);
	void commitBoundary(cocos2d::Touch *touch);
	void checkWinLose();
	void finishRound();

	bool isBoundaryValid(const Line &boundary);
	bool doePlayerWin() const;

	cocos2d::RefPtr<cocos2d::EventListenerTouchOneByOne> m_touchListener;
	cocos2d::RefPtr<cocos2d::DrawNode> m_tempLineNode;
	cocos2d::RefPtr<cocos2d::DrawNode> m_commitedLinesNode;
	cocos2d::RefPtr<GameOverLayer> m_gameOverNode;
	cocos2d::RefPtr<cocos2d::DrawNode> m_debugNode;
	cocos2d::RefPtr<LinesLeftView> m_linesLeftView;
	cocos2d::RefPtr<TimeScoreView> m_timeScoreView;

	Line m_nextBoundary;
	bool m_isNextBoundaryValid = false;
	std::vector<Line> m_boundaries;
	IGameLevelMap *m_map = nullptr;
	GameStatus m_status = GameStatus::Playing;
};
