#pragma once
#include "IHandGenerator.h"

// ============================================================
//  IScoringRule — Interface untuk evaluasi tangan kartu
//  Mutable: bisa diganti tanpa ubah GameSession
// ============================================================
class IScoringRule {
public:
    virtual ~IScoringRule() = default;
    virtual int evaluate(const HandInput& hand) const = 0;
    virtual std::string getRuleName() const = 0;
};

// ============================================================
//  SumScoringRule — Base score = jumlah semua nilai kartu
// ============================================================
class SumScoringRule : public IScoringRule {
public:
    int evaluate(const HandInput& hand) const override {
        int total = 0;
        for (int v : hand.cardValues) total += v;
        return total;
    }
    std::string getRuleName() const override { return "Sum Score"; }
};
