#pragma once

#include <string>

#define PS_EN = 0
#define PS_RU 1
#define PS_LANG PS_RU

namespace l10n
{
#if PS_LANG == PS_RU

static constexpr auto kPlayNow = "Начать игру!";
static constexpr auto kGameTitle = "Отдели коз от капусты!";
static constexpr auto kGameAuthors = "Иван Усков, Сергей Степаненко и Сергей Шамбир\n"
                                     "представляют вам игру...";
static constexpr auto kGameOverWin = "Вы победили!\n"
                                     "Текущий результат: {SCORE} очков";
static constexpr auto kGameOverLose = "Игра окончена\nК сожалению, вы не прошли уровень...";
static constexpr auto kButtonQuit = "Выйти";
static constexpr auto kButtonContinue = "Продолжить";
static constexpr auto kButtonExit = "Выйти";
static constexpr auto kButtonRetry = "Повторить";
static constexpr auto kGameIntro = "Инструкция:\n"
                                   "1) движением пальца по экрану рисуйте линии\n"
                                   "2) используйте линии, чтобы разделить коз и капусту\n"
                                   "3) поторопитесь!";
static constexpr auto kGameIntroCloseButton = "Я понял";

#else

static constexpr auto kPlayNow = "Play Now!";
static constexpr auto kGameTitle = "Stop the Goats!";
static constexpr auto kGameAuthors = "Ivan Uskov, Sergey Stepanenko and Sergey Shambir\n"
                                     "proudly present...";
static constexpr auto kGameOverWin = "Game Over\nCongratulations, you won!\n"
                                     "Your score: {SCORE}";
static constexpr auto kGameOverLose = "Game Over\nUnfortunately, you lose...";
static constexpr auto kButtonQuit = "Quit";
static constexpr auto kButtonContinue = "Continue";
static constexpr auto kButtonExit = "Exit";
static constexpr auto kButtonRetry = "Retry";
static constexpr auto kGameIntro = "How to play:\n"
                                   "1) slide over screen to draw lines\n"
                                   "2) use lines to divide goats and plants\n"
                                   "3) be fast!";
static constexpr auto kGameIntroCloseButton = "Got it!";

#endif

inline std::string expandVariable(std::string text, const std::string &anchor, const std::string &value)
{
    size_t startPos = 0;
    while ((startPos = text.find(anchor, startPos)) != std::string::npos)
    {
        text.replace(startPos, anchor.length(), value.c_str());
        startPos += value.length();
    }
    return text;
}

} // namespace l10n
