#include "OptionsPricing.hpp"
#include <cmath>
#include <vector>
#include <iostream>
#include <random>

using namespace std;

static double normCDF(double x) {
    return 0.5 * erfc(-x * M_SQRT1_2); 
}

void OptionsPricing::blackScholes(double S, double K, double T, double r, double sigma) {
    // S: current price of the underlying asset
    // K: strike price
    // T: time to expiration (in years)
    // r: risk-free interest rate
    // sigma: volatility of the underlying asset

    double d1 = (log(S / K) + (r + 0.5 * sigma * sigma) * T) / (sigma * sqrt(T));
    double d2 = d1 - sigma * sqrt(T);

    double callPrice = S * normCDF(d1) - K * exp(-r * T) * normCDF(d2);
    double putPrice = K * exp(-r * T) * normCDF(-d2) - S * normCDF(-d1);

    cout << "Black-Scholes Call Option Price: " << callPrice << endl;
    cout << "Black-Scholes Put Option Price: " << putPrice << endl;
    cout << " " << endl;
}

void OptionsPricing::binomial(double S, double K, double T, double r, double sigma, int steps) {
    // steps: number of time steps in the binomial tree

    double dt = T / steps;                      // time step size
    double u = exp(sigma * sqrt(dt));           // up factor
    double d = 1 / u;                           // down factor
    double p = (exp(r * dt) - d) / (u - d);     // risk-neutral probability

    vector<double> prices(steps + 1);
    for (int i = 0; i <= steps; ++i) {
        prices[i] = S * pow(u, steps - i) * pow(d, i);
    }

    vector<double> optionValues(steps + 1);
    for (int i = 0; i <= steps; ++i) {
        optionValues[i] = max(0.0, prices[i] - K);
    }

    // Backward induction to calculate the option price at t=0
    for (int step = steps - 1; step >= 0; --step) {
        for (int i = 0; i <= step; ++i) {
            optionValues[i] = exp(-r * dt) * (p * optionValues[i] + (1 - p) * optionValues[i + 1]);
        }
    }

    double callPrice = optionValues[0]; 

    for (int i = 0; i <= steps; ++i) {
        optionValues[i] = max(0.0, K - prices[i]);
    }
    for (int step = steps - 1; step >= 0; --step) {
        for (int i = 0; i <= step; ++i) {
            optionValues[i] = exp(-r * dt) * (p * optionValues[i] + (1 - p) * optionValues[i + 1]);
        }
    }

    double putPrice = optionValues[0];

    cout << "Binomial Call Option Price: " << callPrice << endl;
    cout << "Binomial Put Option Price: " << putPrice << endl;
    cout << " " << endl;
}
