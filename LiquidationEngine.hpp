#pragma once
#include <string>
#include "PositionManager.hpp"
#include "PriceOracle.hpp"

class LiquidationEngine {
public:
    LiquidationEngine(PositionManager &pm, PriceOracle &oracle)
      : pm_(pm), oracle_(oracle) {}

    // If losses exceed (1 - maintMarginRatio) * margin, auto-liquidate
    void checkLiquidation(const std::string &trader,
                          double maintenanceMarginRatio = 0.1);

private:
    PositionManager &pm_;
    PriceOracle &oracle_;
};
