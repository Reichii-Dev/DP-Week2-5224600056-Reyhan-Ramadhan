#pragma once
#include <vector>

// ============================================================
//  HandInput — Data hasil generate kartu untuk satu ronde
// ============================================================
struct HandInput {
    std::vector<int> cardValues;  // nilai kartu yang didealt
};

// ============================================================
//  IHandGenerator — Interface untuk generate tangan kartu
//  Mutable: bisa diganti tanpa ubah GameSession
// ============================================================
class IHandGenerator {
public:
    virtual ~IHandGenerator() = default;
    virtual HandInput generate() const = 0;
};

// ============================================================
//  RandomHandGenerator — Generate 5 kartu random (1-13)
// ============================================================
class RandomHandGenerator : public IHandGenerator {
public:
    HandInput generate() const override {
        HandInput input;
        for (int i = 0; i < 5; i++) {
            input.cardValues.push_back((std::rand() % 13) + 1);
        }
        return input;
    }
};
