#pragma once

// ============================================================
//  IBlindRule — Interface untuk cek apakah score lolos blind
//  Mutable: target blind bisa diubah tanpa ubah GameSession
// ============================================================
class IBlindRule {
public:
    virtual ~IBlindRule() = default;
    virtual bool isPassed(int score, int round) const = 0;
    virtual int getTarget(int round) const = 0;
};

// ============================================================
//  ScalingBlindRule — Target naik tiap ronde
// ============================================================
class ScalingBlindRule : public IBlindRule {
public:
    explicit ScalingBlindRule(int baseTarget = 20, int scaling = 10)
        : baseTarget_(baseTarget), scaling_(scaling) {}

    bool isPassed(int score, int round) const override {
        return score >= getTarget(round);
    }

    int getTarget(int round) const override {
        return baseTarget_ + (round - 1) * scaling_;
    }

private:
    int baseTarget_;
    int scaling_;
};
