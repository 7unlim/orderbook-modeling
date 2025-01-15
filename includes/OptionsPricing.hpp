#ifndef OPTIONSPRICING_HPP
#define OPTIONSPRICING_HPP

class OptionsPricing {
    public:
        void blackScholes(double S, double K, double T, double r, double sigma);
        void binomial(double S, double K, double T, double r, double sigma, int steps);

    private:

};

#endif