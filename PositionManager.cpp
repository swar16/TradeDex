#include "PositionManager.hpp"

void PositionManager::openPosition(const std::string &trader,
                                   double margin,
                                   bool isLong,
                                   double leverage,
                                   double currentPrice) {
    if (margin <= 0 || leverage <= 0) {
        throw std::invalid_argument("Margin and leverage must be positive");
    }
    auto it = positions_.find(trader);
    if (it != positions_.end() && it->second.isOpen) {
        throw std::runtime_error("Position already open for trader");
    }

    // Lock margin from shared vault
    vault_.withdrawCollateral(trader, margin);

    Position pos;
    pos.trader     = trader;
    pos.entryPrice = currentPrice;
    pos.size       = margin * leverage;
    pos.leverage   = leverage;
    pos.isLong     = isLong;
    pos.isOpen     = true;

    positions_[trader] = pos;
}

void PositionManager::closePosition(const std::string &trader,
                                    double currentPrice) {
    auto it = positions_.find(trader);
    if (it == positions_.end() || !it->second.isOpen) {
        throw std::runtime_error("No open position for trader");
    }
    Position &pos = it->second;

    double priceDiff = pos.isLong
        ? (currentPrice - pos.entryPrice)
        : (pos.entryPrice - currentPrice);

    // PnL scaled by notional / entryPrice
    double pnl = priceDiff * (pos.size / pos.entryPrice);

    // Return margin + PnL (never negative)
    double returnAmt = (pos.size / pos.leverage) + pnl;
    if (returnAmt < 0) returnAmt = 0;

    vault_.depositCollateral(trader, returnAmt);
    pos.isOpen = false;
}

Position PositionManager::getPosition(const std::string &trader) const {
    auto it = positions_.find(trader);
    if (it == positions_.end() || !it->second.isOpen) {
        throw std::runtime_error("No open position for trader");
    }
    return it->second;
}
