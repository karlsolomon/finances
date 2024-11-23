#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <print>
#include <string>

#define DEBT_DEBUG false
/**
 * @class Debt
 * @brief Represents a financial debt, including principal, interest rate, and payment tracking.
 */
class Debt {
 private:
 public:
    /**
     * @enum PERIOD_E
     * @brief Enum representing the period of interest accrual.
     */
    using PERIOD_E = enum { PERIOD_MONTHLY, PERIOD_YEARLY, PERIOD_COUNT };

    double principal;              ///< Remaining principal amount of the debt.
    double totalPaid;              ///< Total amount paid toward the debt.
    double rate;                   ///< Annual interest rate (e.g., 0.05 for 5%).
    double minimumMonthlyPayment;  ///< Minimum monthly payment amount.
    PERIOD_E interestPeriod;       ///< Interest accrual period (monthly or yearly).
    int periods;                   ///< Number of payment periods that have elapsed.
    int periodTaken;               ///< The period when the debt starts requiring payments.
    std::string id;  ///< Identifier for the debt (e.g., a loan number). -- debug & debt.csv clarity purposes only

    static constexpr double EPSILON = 0.1;  ///< Threshold for zero comparison.

    Debt(const Debt& d) = default;  // deep copy constructor

    /**
     * @brief Constructor for Debt class.
     * @param p Principal amount.
     * @param r Annual interest rate.
     * @param i Interest accrual period (PERIOD_E).
     * @param id Identifier for the debt.
     * @param minimumMonthlyPayment Minimum payment amount per month.
     * @param periodTaken Period when debt starts requiring payments.
     */
    Debt(double p, double r, PERIOD_E i, std::string id, double minimumMonthlyPayment, int periodTaken);
    /**
     * @brief Gets the remaining principal amount.
     * @return Remaining principal or 0 if payment period has elapsed.
     */
    [[nodiscard]] auto getPrincipal() const -> double;

    /**
     * @brief Gets the total amount paid.
     * @return Total paid amount or 0 if payment period has elapsed.
     */
    [[nodiscard]] auto getTotalPaid() const -> double;
    /**
     * @brief Accrues interest for the current period.
     */
    void accrue();
    /**
     * @brief Makes a payment toward the debt.
     * @param payment Reference to the payment amount. Adjusted after the function.
     */
    void pay(double& payment);
    /**
     * @brief Prints the current status of the debt.
     */
    void print();
    /*auto operator<(const Debt& d) const -> bool { return d.rate < this->rate; }*/
    /*auto operator==(const Debt& d) const -> bool { return d.rate == this->rate; }*/
    /*auto operator>(const Debt& d) const -> bool { return d.rate > this->rate; }*/

    /**
     * @brief Checks if a value is approximately zero.
     * @param d Value to check.
     * @return True if value is close to zero, false otherwise.
     */
    static auto isBasicallyZero(double d) -> bool;

    /**
     * @brief Checks if the debt has a forced minimum payment.
     * @return True if there is a forced minimum payment.
     */
    [[nodiscard]] auto isForced() const -> bool;

    /**
     * @brief Gets the number of periods per year for a given period type.
     * @param p Period type (PERIOD_E).
     * @return Number of periods per year.
     */
    static auto periodsPerYear(PERIOD_E p) -> int;

    /**
     * @brief Converts a period type to a string.
     * @param p Period type (PERIOD_E).
     * @return String representation of the period.
     */
    static auto printPeriod(PERIOD_E p) -> std::string;
    /**
     * @brief Converts periods between different types.
     * @param p1 Source period type.
     * @param p2 Target period type.
     * @return Conversion ratio between periods.
     */
    static auto convertPeriods(PERIOD_E p1, PERIOD_E p2) -> double;
    /**
     * @brief Calculates the compounded total after a number of periods.
     * @param principal Initial principal amount.
     * @param rate Interest rate per period.
     * @param periods Number of periods.
     * @return Compounded total.
     */
    static auto compoundTotal(double principal, double rate, int periods) -> double;
    /**
     * @brief Calculates the compounded interest over a number of periods.
     * @param principal Initial principal amount.
     * @param rate Interest rate per period.
     * @param periods Number of periods.
     * @return Compounded interest.
     */
    static auto compoundInterest(int principal, double rate, int periods) -> double;
};
