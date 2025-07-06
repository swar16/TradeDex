#pragma once
#include <string>
#include <unordered_map>
#include <stdexcept>
#include "CollateralVault.hpp"   // brings in CustomHash

struct Position {
    std::string trader;
    double entryPrice;
    double size;       // notional = margin * leverage
    double leverage;
    bool isLong;
    bool isOpen;
};

class PositionManager {
public:
    // ctor: takes reference to shared collateral vault
    explicit PositionManager(CollateralVault &vault)
      : vault_(vault) {}

    // Open a new position: margin locked, with direction & leverage
    void openPosition(const std::string &trader,
                      double margin,
                      bool isLong,
                      double leverage,
                      double currentPrice);

    // Close an open position, compute PnL, return funds
    void closePosition(const std::string &trader, double currentPrice);

    // Fetch the current open position
    Position getPosition(const std::string &trader) const;

private:
    std::unordered_map<std::string, Position, CustomHash> positions_;
    CollateralVault &vault_;
};
