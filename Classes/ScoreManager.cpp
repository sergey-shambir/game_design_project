#include "ScoreManager.h"
#include <cassert>
#include <cmath>

ScoreManager &ScoreManager::getInstance()
{
	static ScoreManager s_manager;
	return s_manager;
}

unsigned ScoreManager::getLinesLeft() const
{
	return m_linesLeft;
}

unsigned ScoreManager::getScore() const
{
	return m_score;
}

void ScoreManager::updateAfterLinePut()
{
	assert(m_linesLeft > 0);
	if (m_linesLeft > 0)
	{
		--m_linesLeft;
		++m_linesSpentInRound;
	}
}

void ScoreManager::updateAfterLineCanceled()
{
	assert(m_linesSpentInRound > 0);
	if (m_linesSpentInRound > 0)
	{
		++m_linesLeft;
		--m_linesSpentInRound;
	}
}

void ScoreManager::updateBeforeFirstLevel()
{
	constexpr unsigned initialBonusLines = 1;
	m_score = 0;
	m_linesLeft = initialBonusLines;
}

void ScoreManager::updateBeforeRoundStart(const RoundConditions &conditions)
{
	// Добавляем количество линий, необходимых для прохождения уровня.
	m_linesLeft += conditions.linesMin;
	m_linesSpentInRound = 0;
}

void ScoreManager::updateAfterRoundWin(const RoundConditions &conditions, const RoundResults &results)
{
	assert(results.linesSpent == m_linesSpentInRound);
	assert(conditions.linesMin <= results.linesSpent);

	unsigned gainedScore = getGainedScore(conditions, results);
	unsigned bonusLines = getBonusLines(conditions, results);

	m_score += gainedScore;
	m_linesLeft += bonusLines;

	(void)results;
}

unsigned ScoreManager::getGainedScore(const RoundConditions &conditions, const RoundResults &results) const
{
	constexpr unsigned kMaxScoreForSpeed = 100;
	constexpr unsigned kScorePerLine = 15;
	constexpr unsigned kScoreWhenNoExtraLines = 45;

	// Бонус за оставшееся время уровня.
	const float timeFactor = std::max(0.0f, results.secondsSpent / conditions.estimatedSeconds);
	const unsigned timeScore = static_cast<unsigned>(std::round(kMaxScoreForSpeed * timeFactor));

	// Бонус за отсутствие лишних линий
	const unsigned noExtraLinesScore = (results.linesSpent == conditions.linesMin) ? kScoreWhenNoExtraLines : 0;

	// Бонус за количество оставшихся линий.
	const unsigned savedLinesScore = kScorePerLine * m_linesLeft;

	return timeScore + noExtraLinesScore + savedLinesScore;
}

unsigned ScoreManager::getBonusLines(const RoundConditions &conditions, const RoundResults &results) const
{
	const std::pair<float, unsigned> bonusPerTimeFactor[] = {
		{ 0.2f, 2 },
		{ 0.7f, 1 },
	};

	// Бонус за оставшееся время уровня.
	const float timeFactor = std::max(0.0f, results.secondsSpent / conditions.estimatedSeconds);

	for (auto &&pair : bonusPerTimeFactor)
	{
		if (timeFactor < pair.first)
		{
			return pair.second;
		}
	}
	return 0;
}
