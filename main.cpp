#include <iostream>
#include <memory>
#include <cstdlib>
#include <ctime>
#include "GameSession.h"
#include "IHandGenerator.h"
#include "IScoringRule.h"
#include "IRewardRule.h"
#include "IBlindRule.h"

int main() {
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    // --------------------------------------------------------
    //  Semua behavior disuntikkan dari sini.
    //  Untuk ganti behavior, cukup swap class di bawah ini.
    //  GameSession.h tidak perlu diubah sama sekali.
    // --------------------------------------------------------

    auto handGen     = std::make_shared<RandomHandGenerator>();
    auto scoringRule = std::make_shared<SumScoringRule>();
    auto rewardRule  = std::make_shared<BonusRewardRule>();
    auto blindRule   = std::make_shared<ScalingBlindRule>(20, 10);

    GameSession game(handGen, scoringRule, rewardRule, blindRule);
    game.startGame();

    return 0;
}
