#include "Debt.hpp"

#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <print>

#define DEBT_PRINT(...) \
    if (DEBT_DEBUG) std::println(__VA_ARGS__)

/**
 * @class Debt
 * @brief Represents a financial debt, including principal, interest rate, and payment tracking.
 */
Debt::Debt(double p, double r, PERIOD_E i, std::string id, double minimumMonthlyPayment, int periodTaken) {
    this->principal = p;
    this->rate = r;
    this->interestPeriod = i;
    this->totalPaid = 0.0;
    this->id = id;
    this->periods = 0;
    this->periodTaken = periodTaken;
    this->minimumMonthlyPayment = minimumMonthlyPayment;
}

/**
 * @brief Gets the remaining principal amount.
 * @return Remaining principal or 0 if payment period has elapsed.
 */
auto Debt::getPrincipal() const -> double {
    if (this->periodTaken <= this->periods) {
        return this->principal;
    }
    return 0.0;
}

/**
 * @brief Gets the total amount paid.
 * @return Total paid amount or 0 if payment period has elapsed.
 */
auto Debt::getTotalPaid() const -> double {
    if (this->periodTaken <= this->periods) {
        return this->totalPaid;
    }
    return 0.0;
}

/**
 * @brief Accrues interest for the current period.
 */
void Debt::accrue() {
    this->periods++;
    if (this->periodTaken <= this->periods) {
        if ((this->periods % periodsPerYear(this->interestPeriod)) == 0) {
            this->principal = compoundTotal(this->principal, this->rate, 1);
        }
    }
}

/**
 * @brief Makes a payment toward the debt.
 * @param payment Reference to the payment amount. Adjusted after the function.
 */
void Debt::pay(double& payment) {
    if (this->periodTaken <= this->periods) {
        if (payment > this->principal) {
            this->totalPaid += this->principal;
            payment -= this->principal;
            this->principal = 0.0;
            DEBT_PRINT("{} paid off with {:.2f} USD", this->id, this->totalPaid);
        } else {
            this->totalPaid += payment;
            this->principal -= payment;
            payment = 0.0;
        }
    }
}

/**
 * @brief Prints the current status of the debt.
 */
void Debt::print() {
    DEBT_PRINT("{}: ${} remaining at {:.2f}% per {} with ${} paid so far", this->id, this->principal,
               this->rate * 100.0, printPeriod(this->interestPeriod), this->totalPaid);
}

/*auto operator<(const Debt& d) const -> bool { return d.rate < this->rate; }*/
/*auto operator==(const Debt& d) const -> bool { return d.rate == this->rate; }*/
/*auto operator>(const Debt& d) const -> bool { return d.rate > this->rate; }*/

/**
 * @brief Checks if a value is approximately zero.
 * @param d Value to check.
 * @return True if value is close to zero, false otherwise.
 */
auto Debt::isBasicallyZero(double d) -> bool { return (std::abs(d) <= EPSILON); }

/**
 * @brief Checks if the debt has a forced minimum payment.
 * @return True if there is a forced minimum payment.
 */
auto Debt::isForced() const -> bool { return this->minimumMonthlyPayment > 0.0; }

/**
 * @brief Gets the number of periods per year for a given period type.
 * @param p Period type (PERIOD_E).
 * @return Number of periods per year.
 */
auto Debt::periodsPerYear(PERIOD_E p) -> int {
    switch (p) {
        case PERIOD_MONTHLY:
            return 1;
        case PERIOD_YEARLY:
            return 12;
        default:
            return 0;
    }
}

/**
 * @brief Converts a period type to a string.
 * @param p Period type (PERIOD_E).
 * @return String representation of the period.
 */
auto Debt::printPeriod(PERIOD_E p) -> std::string {
    switch (p) {
        case PERIOD_MONTHLY:
            return "month";
        case PERIOD_YEARLY:
            return "year";
        default:
            return "invalid";
    }
}

/**
 * @brief Converts periods between different types.
 * @param p1 Source period type.
 * @param p2 Target period type.
 * @return Conversion ratio between periods.
 */
auto Debt::convertPeriods(PERIOD_E p1, PERIOD_E p2) -> double {
    return (static_cast<double>(periodsPerYear(p1))) / (static_cast<double>(periodsPerYear(p2)));
}

/**
 * @brief Calculates the compounded total after a number of periods.
 * @param principal Initial principal amount.
 * @param rate Interest rate per period.
 * @param periods Number of periods.
 * @return Compounded total.
 */
auto Debt::compoundTotal(double principal, double rate, int periods) -> double {
    return principal * std::pow(1.0 + rate, static_cast<double>(periods));
}

/**
 * @brief Calculates the compounded interest over a number of periods.
 * @param principal Initial principal amount.
 * @param rate Interest rate per period.
 * @param periods Number of periods.
 * @return Compounded interest.
 */
auto Debt::compoundInterest(int principal, double rate, int periods) -> double {
    double total = compoundTotal(principal, rate, periods);
    double interest = total - static_cast<double>(principal);
    return interest;
}
