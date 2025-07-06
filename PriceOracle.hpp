#pragma once

class PriceOracle {
public:
    // Off-chain service will call this to push new market price
    void updatePrice(double price);

    // On-chain modules call this to read the latest price
    double getPrice() const;

private:
    double latestPrice_ = 0.0;
};
