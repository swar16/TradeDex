#include "PriceOracle.hpp"
#include <stdexcept>

void PriceOracle::updatePrice(double price) {
    if (price <= 0) {
        throw std::invalid_argument("Price must be positive");
    }
    latestPrice_ = price;
}

double PriceOracle::getPrice() const {
    return latestPrice_;
}
