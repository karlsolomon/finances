/**
 * @file Worker.hpp
 * @brief Defines a Worker class to simulate financial operations and debt payment strategies.
 */

#include "Worker.hpp"

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <functional>
#include <iostream>
#include <mutex>
#include <ostream>
#include <print>

#include "flags.hpp"

static constexpr double paymentGrowthRate = 0.5;                  ///< Multiplier on payment range after promotion.
static constexpr int paymentGrowthFrequency = 36;                 ///< Promotion or job change cadence (in periods).
static constexpr double paymentMin = 2000.0 + AGGRESSIVE_OFFSET;  ///< Minimum payment with offset.
static constexpr double paymentMax = 3000.0 + AGGRESSIVE_OFFSET;  ///< Maximum payment with offset.

// Out-of-line static initialization
std::mutex Worker::rngMutex = std::mutex();
std::vector<Debt> Worker::masterDebt = {};
std::mt19937 Worker::gen = std::mt19937(std::random_device()());
std::uniform_real_distribution<double> Worker::distr = std::uniform_real_distribution<double>(paymentMin, paymentMax);

/**
 * @brief Main simulation function for the worker.
 */
void Worker::run() {
    std::ofstream file;
    file.open(FILE_PREFIX + std::to_string(this->id) + ".csv");
    std::vector<Debt> debts;
    int periods = 0;
    for (int i = 0; i < this->iter; i++) {
        for (auto d : masterDebt) {
            debts.emplace_back(d.principal, d.rate, d.interestPeriod, d.id, d.minimumMonthlyPayment, d.periodTaken);
        }

        // sort by decreasing interest rate
        std::ranges::sort(debts, std::ranges::greater(), &Debt::rate);
        for (auto& d : debts) {
            d.periods = 0;
        }
        periods = 0;
        double totalPaid = 0.0;
        while (true) {
            DEBUG_PRINT("{:.2f},{:.2f}", getTotalDebt(debts), getTotalPaid(debts) + totalPaid);
            for (auto& d : debts) {
                d.accrue();
            }

            double payment = getRandom(periods);
            payForcedDebt(debts, payment);
            payNonForcedDebt(debts, payment);

            for (auto& d : debts) {
                if (Debt::isBasicallyZero(d.principal)) {
                    totalPaid += d.totalPaid;
                }
            }

            auto isZero = [](Debt& d) { return Debt::isBasicallyZero(d.principal); };
            auto new_end = std::ranges::remove_if(debts, isZero);
            debts.erase(new_end.begin(), debts.end());
            periods++;
#if (KID)
            if ((debts.size() == 1) && (debts[0].id == "\"kid\"")) {
                // for the purposes of this exercise we're only interested in when we pay off the student loans, not
                // when we acquire enough money to stash away to fully raise the child
                debts.clear();
                break;
            }
#endif

            if (!Debt::isBasicallyZero(payment)) {
                break;
            }
        }
        file << std::format("{:.2f}", totalPaid) << "," << periods << std::endl;
    }
    file.close();
}

/**
 * @brief Starts the worker thread.
 */
void Worker::start() { t = std::thread(&Worker::run, this); }

/**
 * @brief Joins the worker thread to the main thread.
 */
void Worker::join() { t.join(); }

/**
 * @brief Sets the shared debt configuration for all workers.
 * @param d Reference to the debt vector.
 */
void Worker::setMasterDebt(std::vector<Debt>& d) { Worker::masterDebt = d; }

/**
 * @brief Calculates a random payment amount based on the period.
 * @param period The current simulation period.
 * @return Random payment amount.
 */
auto Worker::getRandom(int period) -> double {
    std::lock_guard<std::mutex> lock(rngMutex);
    static bool first = true;
    if (first) {
        std::pair<double, double> p = getPayRange(period);
        first = false;
        distr = std::uniform_real_distribution<double>(p.first, p.second);
    }

    return distr(gen);
}

/**
 * @brief Calculates a range of payment amounts based on the simulation period.
 * @param periods The current number of periods elapsed.
 * @return A pair representing the minimum and maximum payment amounts.
 */
auto Worker::getPayRange(int periods) -> std::pair<double, double> {
    double growth = 1.0 + (paymentGrowthRate * std::floor((static_cast<double>(periods) / paymentGrowthFrequency)));
    return {paymentMin * growth, paymentMax * growth};
}

/**
 * @brief Pays off forced debts using available payment.
 * @param debts Reference to the vector of debts.
 * @param payment Reference to the payment amount.
 */
void Worker::payForcedDebt(std::vector<Debt>& debts, double& payment) {
#if AGGRESSIVE
    // Use payment towards forced debts
    for (auto& d : debts) {
        if (d.isForced() && (d.periods >= d.periodTaken)) {
            double forced = d.minimumMonthlyPayment;
            d.pay(forced);
            payment -= (d.minimumMonthlyPayment - forced);
        }
    }
#else
    // Do not use payment towards forced debts; treat as QoL boost
    for (auto& d : debts) {
        if (d.isForced() && (d.periods >= d.periodTaken)) {
            d.pay(d.minimumMonthlyPayment);
        }
    }
#endif
}

/**
 * @brief Pays off non-forced debts using available payment.
 * @param debts Reference to the vector of debts.
 * @param payment Reference to the payment amount.
 */
void Worker::payNonForcedDebt(std::vector<Debt>& debts, double& payment) {
    for (auto& d : debts) {
        // Only pay minimum monthly payments on forced debts
        if (!d.isForced()) {
            d.pay(payment);
            if (Debt::isBasicallyZero(payment)) {
                return;
            }
        }
    }

    // If all non-forced debts are zero, pay off forced debts early
    for (auto& d : debts) {
        d.pay(payment);
    }
}

/**
 * @brief Calculates the total debt from a vector of debts.
 * @param debts Reference to the vector of debts.
 * @return Total debt amount.
 */
auto Worker::getTotalDebt(std::vector<Debt>& debts) -> double {
    double res = 0.0;
    for (auto d : debts) {
        res += d.getPrincipal();
    }
    return res;
}

/**
 * @brief Calculates the total paid amount from a vector of debts.
 * @param debts Reference to the vector of debts.
 * @return Total paid amount.
 */
auto Worker::getTotalPaid(std::vector<Debt>& debts) -> double {
    double res = 0.0;
    for (auto d : debts) {
        res += d.getTotalPaid();
    }
    return res;
}
