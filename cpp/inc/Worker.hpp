/**
 * @file Worker.hpp
 * @brief Defines a Worker class to simulate financial operations and debt payment strategies.
 */

#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <mutex>
#include <random>
#include <thread>
#include <vector>

#include "Debt.hpp"

/**
 * @class Worker
 * @brief Represents a worker thread simulating debt payment and financial decisions.
 */
class Worker {
 private:
    int iter;                                             ///< Number of iterations the worker will perform.
    int id;                                               ///< Unique ID of the worker thread.
    std::thread t;                                        ///< Thread object associated with the worker.
    static std::mt19937 gen;                              ///< Random number generator for payment amounts.
    static std::mutex rngMutex;                           ///< Mutex for thread-safe RNG access.
    static std::uniform_real_distribution<double> distr;  ///< Distribution for random payments.
    static std::vector<Debt> masterDebt;                  ///< Shared debt configuration across all workers.

 public:
    /**
     * @brief Constructs a Worker object.
     * @param iter Number of iterations to perform.
     * @param id Unique ID for the worker.
     */
    Worker(int iter, int id) : iter(iter), id(id) {}

    /**
     * @brief Main simulation function for the worker.
     */
    void run();
    /**
     * @brief Starts the worker thread.
     */
    void start();
    /**
     * @brief Joins the worker thread to the main thread.
     */
    void join();
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
};
