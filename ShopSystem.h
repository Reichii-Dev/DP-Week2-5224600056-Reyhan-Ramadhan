#pragma once
#include <iostream>
#include <string>

// ============================================================
//  ShopSystem — Toko antara ronde
//  Mutable: item dan harga bisa diubah tanpa ubah GameSession
// ============================================================
class ShopSystem {
public:
    void open(int& chips) const {
        std::cout << "[SHOP] --- Shop is open! ---\n";
        std::cout << "[SHOP] offered: ChipBoost(+15) cost 10\n";

        if (chips < 10) {
            std::cout << "[SHOP] Not enough chips. Skipped.\n";
            return;
        }

        std::cout << "[SHOP] Enter 'buy' to purchase or anything to skip: ";
        std::string input;
        std::cin >> input;

        if (input == "buy") {
            chips -= 10;
            chips += 15;
            std::cout << "[SHOP] Purchased! chips: " << chips << "\n";
        } else {
            std::cout << "[SHOP] Skipped.\n";
        }
    }
};
