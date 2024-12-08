/**
 * @file Worker.hpp
 * @brief Defines a Worker class to simulate financial operations and debt payment strategies.
 */

#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <mutex>
#include <random>
#include <vector>

#include "Debt.hpp"

/**
 * @class Worker
 * @brief Represents a worker thread simulating debt payment and financial decisions.
 */
class Worker {
 private:
    int id;                                               ///< Unique ID of the worker thread.
    static std::mt19937 gen;                              ///< Random number generator for payment amounts.
    static std::mutex rngMutex;                           ///< Mutex for thread-safe RNG access.
    static std::uniform_real_distribution<double> distr;  ///< Distribution for random payments.
    static std::vector<Debt> masterDebt;                  ///< Shared debt configuration across all workers.
    double totalPaid = 0.0;
    unsigned int periods = 0;

 public:
    /**
     * @brief Constructs a Worker object.
     * @param id Unique ID for the worker.
     */
    Worker(int id) : id(id) {}

    [[nodiscard]] auto getTotalPaid() const -> double { return this->totalPaid; }
    [[nodiscard]] auto getPeriods() const -> double { return this->periods; }

    /**
     * @brief Main simulation function for the worker.
     */
    void run();
    /**
     * @brief Sets the shared debt configuration for all workers.
     * @param d Reference to the debt vector.
     */
    static void setMasterDebt(std::vector<Debt>& d);

    /**
     * @brief Calculates a random payment amount based on the period.
     * @param period The current simulation period.
     * @return Random payment amount.
     */
    static auto getRandom(int period) -> double;
    /**
     * @brief Calculates a range of payment amounts based on the simulation period.
     * @param periods The current number of periods elapsed.
     * @return A pair representing the minimum and maximum payment amounts.
     */
    static auto getPayRange(int periods) -> std::pair<double, double>;
    /**
     * @brief Pays off forced debts using available payment.
     * @param debts Reference to the vector of debts.
     * @param payment Reference to the payment amount.
     */
    static void payForcedDebt(std::vector<Debt>& debts, double& payment);
    /**
     * @brief Pays off non-forced debts using available payment.
     * @param debts Reference to the vector of debts.
     * @param payment Reference to the payment amount.
     */
    static void payNonForcedDebt(std::vector<Debt>& debts, double& payment);
    /**
     * @brief Calculates the total debt from a vector of debts.
     * @param debts Reference to the vector of debts.
     * @return Total debt amount.
     */
    static auto getTotalDebt(std::vector<Debt>& debts) -> double;
    /**
     * @brief Calculates the total paid amount from a vector of debts.
     * @param debts Reference to the vector of debts.
     * @return Total paid amount.
     */
    static auto getTotalPaid(std::vector<Debt>& debts) -> double;

    static auto getTotalInterestPayment(std::vector<Debt>& debts) -> double;
};
