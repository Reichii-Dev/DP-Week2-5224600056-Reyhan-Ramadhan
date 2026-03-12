# Task 2 — Core Loop Architecture (Card Run Game)

Based on the Balatro-inspired card game from the Pretest.

---

## Step 1 — Core Loop

The core loop of my card run game:

1. Player draws a hand of 5 cards
2. System evaluates the hand and computes a base score
3. System checks if score beats the blind (win/lose condition)
4. Reward (chips) is calculated from the base score
5. Game state updates (chips added, round advances)
6. Shop phase — player can spend chips for upgrades
7. Repeat for next round, or end if all rounds complete / blind failed

---

## Step 2 — Invariant Structure

**Steps that must NEVER change order:**

1. Draw must happen before evaluate — you cannot score a hand that doesn't exist yet
2. Evaluate must happen before blind check — you need a score to check against the target
3. Blind check must happen before reward — no reward should be given if the round is failed
4. Reward must happen before state update — chips come from the reward, not from nowhere
5. State update must happen before shop — shop needs accurate chip count to validate purchases
6. Shop must happen before round advance — shop is a between-round phase, not mid-round

**Components that must always exist:**

- `IHandGenerator` — without input, there is nothing to evaluate
- `IScoringRule` — without scoring, blind check is impossible
- `IBlindRule` — without a win/lose condition, the game has no stakes
- `IRewardRule` — without reward, progression is meaningless
- `GameSession` — the container that enforces the phase order

**What breaks if order changes:**

If blind check happens after reward, the player receives chips even when they lose — breaking game balance. If evaluate happens after blind check, the score doesn't exist yet and the check has nothing to compare against. The entire game logic collapses if phase order is violated.

---

## Step 3 — Mutable Elements

| Element | Class | Why Mutable |
|---|---|---|
| Hand generation | `IHandGenerator` | Can be fixed, random, or loaded from file without affecting loop |
| Scoring formula | `IScoringRule` | Can be sum, high card, poker hand — swappable per game mode |
| Reward calculation | `IRewardRule` | Can add flat bonus, multiply, or scale with round — changes economy feel |
| Blind target scaling | `IBlindRule` | Can scale fast, slow, or stay fixed — affects difficulty curve |
| Shop items & prices | `ShopSystem` | Items and costs can change without touching game loop |

These are mutable because they represent **what** the game does, not **when** it does it. The loop structure (order of phases) is invariant, but the content of each phase can change freely.

---

## Step 4 — C++ Core Loop Skeleton

See `GameSession.h` for the full implementation.

Key structure:
```cpp
class GameSession {
public:
    void startGame() {
        while (!isGameOver()) {
            playRound();
        }
    }

private:
    void playRound() {
        HandInput hand    = handGen_->generate();      // Phase 1
        int baseScore     = scoringRule_->evaluate(hand); // Phase 2
        if (!blindRule_->isPassed(baseScore, round))   // Phase 3
            { gameOver_ = true; return; }
        int reward        = rewardRule_->compute(baseScore); // Phase 4
        chips_           += reward;                    // Phase 5
        shop_.open(chips_);                            // Phase 6
        currentRound_++;                               // Phase 7
    }
};
```

---

## Architecture Overview

| Class | Role |
|---|---|
| `GameSession` | Enforces invariant phase order, owns game state |
| `IHandGenerator` / `RandomHandGenerator` | Generates 5 random card values |
| `IScoringRule` / `SumScoringRule` | Sums card values for base score |
| `IRewardRule` / `BonusRewardRule` | Adds +10 bonus on top of base score |
| `IBlindRule` / `ScalingBlindRule` | Target increases by 10 each round |
| `ShopSystem` | Offers chip upgrade between rounds |

---

## Reflection

### 1. What is the invariant structure of your game?

The invariant structure is the phase order enforced inside `GameSession::playRound()`. Every round must execute these phases in exact sequence: draw hand → evaluate score → check blind → compute reward → update chips → open shop → advance round. This order is non-negotiable. The game's correctness depends entirely on it. A score cannot be evaluated before a hand is drawn. A blind cannot be checked before a score exists. A reward cannot be granted before the blind is passed. Breaking any of these dependencies produces an incorrect or broken game state.

The invariant is protected by keeping all logic outside of `GameSession`. The session only calls interfaces. It never decides how to generate cards, how to score them, or how to reward the player. It only decides when each phase runs.

### 2. What parts are mutable?

The mutable parts are the four injected dependencies: `IHandGenerator`, `IScoringRule`, `IRewardRule`, and `IBlindRule`. Each can be replaced with a different implementation without touching `GameSession`. The shop items and prices inside `ShopSystem` are also mutable — the shop can offer different upgrades in different game modes without affecting the loop structure. Blind target values, reward formulas, and scoring rules are all content, not structure, and content is always mutable.

### 3. If you wanted to add a new feature, which class would change?

It depends on what the feature is. Adding a new scoring mode (example: high card scoring) only requires creating a new class that implements `IScoringRule` and injecting it in `main.cpp`. Adding a new reward behavior only requires a new `IRewardRule` implementation. Adding a new shop item only requires editing `ShopSystem`. In all these cases, `GameSession` does not change. The only scenario where `GameSession` would need to change is if a completely new phase needed to be added to the loop — for example, a "discard phase" between draw and evaluate. That would be an architectural change, not a content change.

### 4. If you changed the loop order, what would break?

If reward was computed before blind check: players would gain chips even on a failed round, making the game unwinnable in terms of meaningful consequence. If shop opened before reward: chips would not yet reflect the current round's earnings, so purchase validation would be wrong. If blind check happened before evaluation: there would be no score to compare against the target. If hand generation was skipped or moved after evaluation: the scoring system would receive empty or stale data. Every phase exists because the next phase depends on it. The loop is a data pipeline — disorder destroys the pipeline.

---

## How to Build & Run

1. Open Visual Studio 2022
2. Create new Console Application (C++) project
3. Add all `.h` files to Header Files, `main.cpp` to Source Files
4. Remove the default `.cpp` file VS creates
5. Build (`Ctrl+Shift+B`) and Run (`Ctrl+F5`)

---

## AI Usage Disclosure

AI (Claude by Anthropic) was used as a learning assistant to explain invariant vs mutable architecture, dependency injection, and interface design in C++. All code was reviewed and understood by the student before submission.
