#include "CollateralVault.hpp"

void CollateralVault::depositCollateral(const std::string &user, double amount) {
    if (amount <= 0) {
        throw std::invalid_argument("Deposit amount must be positive");
    }
    balances_[user].totalCollateral += amount;
}

void CollateralVault::withdrawCollateral(const std::string &user, double amount) {
    auto it = balances_.find(user);
    if (it == balances_.end() || it->second.totalCollateral < amount) {
        throw std::runtime_error("Insufficient collateral for withdrawal");
    }
    if (amount <= 0) {
        throw std::invalid_argument("Withdrawal amount must be positive");
    }
    it->second.totalCollateral -= amount;
}

double CollateralVault::getBalance(const std::string &user) const {
    auto it = balances_.find(user);
    return (it == balances_.end()) ? 0.0 : it->second.totalCollateral;
}
