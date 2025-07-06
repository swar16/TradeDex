#include "LiquidationEngine.hpp"

void LiquidationEngine::checkLiquidation(const std::string &trader,
                                         double maintenanceMarginRatio) {
    try {
        Position pos = pm_.getPosition(trader);
        double price = oracle_.getPrice();

        double priceDiff = pos.isLong
            ? (price - pos.entryPrice)
            : (pos.entryPrice - price);

        double unrealizedPnL = priceDiff * (pos.size / pos.entryPrice);
        double margin        = pos.size / pos.leverage;

        // If losses > allowed, liquidate
        if (unrealizedPnL < -margin * (1 - maintenanceMarginRatio)) {
            pm_.closePosition(trader, price);
        }
    } catch (...) {
        // No open position or already closed—do nothing
    }
}
