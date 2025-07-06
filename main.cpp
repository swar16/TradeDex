#include <iostream>
#include <vector>
#include <string>

#include "CollateralVault.hpp"
#include "PriceOracle.hpp"
#include "PositionManager.hpp"
#include "LiquidationEngine.hpp"

int main() {
    CollateralVault vault;
    PriceOracle    oracle;
    PositionManager pm(vault);
    LiquidationEngine engine(pm, oracle);

    // 1) Deposit
    vault.depositCollateral("alice", 200.0);
    vault.depositCollateral("bob",   150.0);
    std::cout << "=== Initial Balances ===\n"
              << "Alice: " << vault.getBalance("alice") << "\n"
              << "Bob:   " << vault.getBalance("bob")   << "\n\n";

    // 2) Feed in initial price
    oracle.updatePrice(1000.0);
    std::cout << "Market price set to: " << oracle.getPrice() << "\n";

    // 3) Open 2x Long (Alice), 3x Short (Bob)
    pm.openPosition("alice", 100.0, true,  2.0, oracle.getPrice());
    pm.openPosition("bob",   50.0,  false, 3.0, oracle.getPrice());
    std::cout << "\nPositions opened.\n";

    // 4) Simulate price changes + liquidation
    for (double p : std::vector<double>{950.0, 1100.0, 800.0}) {
        oracle.updatePrice(p);
        std::cout << "\nPrice updated to: " << oracle.getPrice() << "\n";
        engine.checkLiquidation("alice");
        engine.checkLiquidation("bob");
        std::cout << "Balances:\n"
                  << " Alice: " << vault.getBalance("alice") << "\n"
                  << " Bob:   " << vault.getBalance("bob")   << "\n";
    }

    // 5) Close remaining
    try {
        double finalP = oracle.getPrice();
        pm.closePosition("alice", finalP);
        pm.closePosition("bob",   finalP);
    } catch (const std::exception &e) {
        std::cerr << "Close error: " << e.what() << "\n";
    }

    // 6) Withdraw all
    try {
        vault.withdrawCollateral("alice", vault.getBalance("alice"));
        vault.withdrawCollateral("bob",   vault.getBalance("bob"));
    } catch (const std::exception &e) {
        std::cerr << "Withdraw error: " << e.what() << "\n";
    }

    // Final
    std::cout << "\n=== Final Summary ===\n"
              << "Alice: " << vault.getBalance("alice") << "\n"
              << "Bob:   " << vault.getBalance("bob")   << "\n";

    return 0;
}
