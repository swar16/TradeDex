#pragma once
#include <string>
#include <unordered_map>
#include <stdexcept>
#include <cstdint>

// Custom hash to reduce collision attacks
struct CustomHash {
    static uint64_t splitmix64(uint64_t x) {
        x += 0x9e3779b97f4a7c15;
        x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9;
        x = (x ^ (x >> 27)) * 0x94d049bb133111eb;
        return x ^ (x >> 31);
    }

    size_t operator()(const std::string &s) const noexcept {
        static std::hash<std::string> hasher;
        return splitmix64(hasher(s));
    }
};

struct UserBalance {
    double totalCollateral = 0.0;
};

class CollateralVault {
public:
    // Deposit collateral for a user
    void depositCollateral(const std::string &user, double amount);

    // Withdraw collateral if sufficient funds available
    void withdrawCollateral(const std::string &user, double amount);

    // Query current balance
    double getBalance(const std::string &user) const;

private:
    std::unordered_map<std::string, UserBalance, CustomHash> balances_;
};
