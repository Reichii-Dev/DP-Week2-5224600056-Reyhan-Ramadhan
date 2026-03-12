#pragma once
#include <iostream>
#include <memory>
#include <numeric>
#include "IHandGenerator.h"
#include "IScoringRule.h"
#include "IRewardRule.h"
#include "IBlindRule.h"
#include "ShopSystem.h"

// ============================================================
//  GameSession — Core loop controller (INVARIANT)
//
//  Phase order MUST NEVER change:
//  1. Player draws hand (generate input)
//  2. System evaluates hand (compute score)
//  3. Check blind (win/lose condition)
//  4. Compute reward (chips gained)
//  5. Update game state (chips, round)
//  6. Shop phase
//  7. Advance round / check game over
//
//  GameSession does NOT contain:
//  - Hand generation logic
//  - Scoring logic
//  - Reward logic
//  - Blind target logic
// ============================================================
class GameSession {
public:
    GameSession(
        std::shared_ptr<IHandGenerator> handGen,
        std::shared_ptr<IScoringRule>   scoringRule,
        std::shared_ptr<IRewardRule>    rewardRule,
        std::shared_ptr<IBlindRule>     blindRule
    )
        : handGen_(handGen),
          scoringRule_(scoringRule),
          rewardRule_(rewardRule),
          blindRule_(blindRule),
          chips_(0),
          totalRounds_(3),
          currentRound_(1),
          gameOver_(false)
    {}

    void startGame() {
        std::cout << "=== CARD RUN GAME START ===\n";
        std::cout << "Survive " << totalRounds_ << " rounds!\n\n";

        while (!isGameOver()) {
            playRound();
        }

        std::cout << "\n=== GAME END ===\n";
        std::cout << "Final chips: " << chips_ << "\n";
    }

private:
    // --------------------------------------------------------
    //  playRound() — One full round, phases 1-7
    // --------------------------------------------------------
    void playRound() {
        std::cout << "-------------------------------\n";
        std::cout << "Round " << currentRound_ << " / " << totalRounds_;
        std::cout << " | Blind Target: "
                  << blindRule_->getTarget(currentRound_) << "\n";
        std::cout << "-------------------------------\n";

        // Phase 1 — Player draws hand
        HandInput hand = handGen_->generate();
        std::cout << "[DRAW] hand: [ ";
        for (int v : hand.cardValues) std::cout << v << " ";
        std::cout << "]\n";

        // Phase 2 — System evaluates hand (compute score)
        int baseScore = scoringRule_->evaluate(hand);
        std::cout << "[EVAL] base score (" << scoringRule_->getRuleName()
                  << "): " << baseScore << "\n";

        // Phase 3 — Check blind (win/lose condition)
        if (!blindRule_->isPassed(baseScore, currentRound_)) {
            std::cout << "[BLIND] FAILED! Score too low. Game over.\n";
            gameOver_ = true;
            return;
        }
        std::cout << "[BLIND] Passed!\n";

        // Phase 4 — Compute reward
        int reward = rewardRule_->compute(baseScore);

        // Phase 5 — Update game state
        chips_ += reward;
        std::cout << "[REWARD] gained: " << reward
                  << " | total chips: " << chips_ << "\n";

        // Phase 6 — Shop phase
        if (currentRound_ < totalRounds_) {
            shop_.open(chips_);
        }

        // Phase 7 — Advance round
        currentRound_++;
        if (currentRound_ > totalRounds_) {
            gameOver_ = true;
        }
    }

    bool isGameOver() const { return gameOver_; }

    // --- Dependencies (mutable behavior) ---
    std::shared_ptr<IHandGenerator> handGen_;
    std::shared_ptr<IScoringRule>   scoringRule_;
    std::shared_ptr<IRewardRule>    rewardRule_;
    std::shared_ptr<IBlindRule>     blindRule_;
    ShopSystem shop_;

    // --- Game state ---
    int chips_;
    int totalRounds_;
    int currentRound_;
    bool gameOver_;
};
