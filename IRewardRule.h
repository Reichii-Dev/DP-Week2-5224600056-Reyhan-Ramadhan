#pragma once

// ============================================================
//  IRewardRule — Interface untuk hitung reward dari score
//  Mutable: bisa diganti tanpa ubah GameSession
// ============================================================
class IRewardRule {
public:
    virtual ~IRewardRule() = default;
    virtual int compute(int baseScore) const = 0;
};

// ============================================================
//  StandardRewardRule — Reward = base score langsung
// ============================================================
class StandardRewardRule : public IRewardRule {
public:
    int compute(int baseScore) const override {
        return baseScore;
    }
};

// ============================================================
//  BonusRewardRule — Reward = base score + 10 bonus chips
// ============================================================
class BonusRewardRule : public IRewardRule {
public:
    int compute(int baseScore) const override {
        return baseScore + 10;
    }
};
