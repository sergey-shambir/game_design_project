#pragma once

struct RoundConditions
{
	unsigned linesMin = 0;
	float estimatedSeconds = 0.0f; // estimated time to pass scene, in seconds.
};

struct RoundResults
{
	float secondsSpent = 0.0f; // seconds spent on scene.
	unsigned linesSpent = 0;
};

class ScoreManager
{
public:
	static ScoreManager &getInstance();

	unsigned getLinesLeft() const;
	unsigned getScore() const;
	bool didShowIntro() const;

	void updateAfterIntroShown();
	void updateAfterLinePut();
	void updateAfterLineCanceled();

	void updateBeforeFirstLevel();
	void updateBeforeRoundStart(const RoundConditions &conditions);
	void updateAfterRoundWin(const RoundConditions &conditions, const RoundResults &results);

private:
	unsigned getGainedScore(const RoundConditions &conditions, const RoundResults &results) const;
	unsigned getBonusLines(const RoundConditions &conditions, const RoundResults &results) const;

	bool m_didShowIntro = false;
	unsigned m_score = 0;
	unsigned m_linesLeft = 0;
	unsigned m_linesSpentInRound = 0;
};
