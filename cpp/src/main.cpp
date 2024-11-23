/**
 * @file main.cpp
 * @brief Entry point for the simulation program. Configures workers, parses data, and runs simulations.
 */

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iostream>
#include <ostream>
#include <print>
#include <thread>
#include <vector>

#include "CsvParser.hpp"
#include "Worker.hpp"
#include "flags.hpp"

/**
 * @brief Converts yearly interest rate to a monthly rate for a given debt.
 * @param d Reference to the debt object.
 */
void convertToMonthly(Debt& d) {
    d.rate = d.rate / Debt::convertPeriods(d.interestPeriod, Debt::PERIOD_MONTHLY);
    d.interestPeriod = Debt::PERIOD_MONTHLY;
}

/**
 * @brief Calculates the total non-forced debt from a vector of debts.
 * @param debts Reference to the vector of debts.
 * @return Total non-forced debt amount.
 */
auto getTotalNonForcedDebt(std::vector<Debt>& debts) -> double {
    double res = 0.0;
    for (auto d : debts) {
        if (!Debt::isBasicallyZero(d.minimumMonthlyPayment)) {
            res += d.getPrincipal();
        }
    }
    return res;
}

/**
 * @brief Calculates the total owed on non-forced debts from a vector of debts.
 * @param debts Reference to the vector of debts.
 * @return Total owed amount on non-forced debts.
 */
auto getTotalOwedNonForced(std::vector<Debt>& debts) -> double {
    double res = 0.0;
    for (auto d : debts) {
        if (!d.isForced()) {
            res += d.principal;
        }
    }
    return res;
}

/**
 * @brief Gets the count of non-forced debts from a vector of debts.
 * @param debts Reference to the vector of debts.
 * @return Count of non-forced debts.
 */
auto getNumNonForced(std::vector<Debt>& debts) -> int {
    int res = 0;
    for (auto d : debts) {
        if (!d.isForced()) {
            res++;
        }
    }
    return res;
}

/**
 * @brief Entry point of the simulation program.
 * Initializes the workers, parses CSV data, and combines simulation results.
 * @return Exit code (0 for success).
 */
auto main() -> int {
    DEBUG_PRINT("creating {} threads", std::thread::hardware_concurrency());

    CsvParser csv("../debt.csv");
    std::ofstream simulations;
    std::vector<Worker> workers;
    static std::vector<Debt> masterDebt;
#if (DEBUG)
    unsigned int numWorkers = 1;
#else
    unsigned int numWorkers = std::thread::hardware_concurrency();
#endif
    // Parse CSV File
    if (auto data = csv.parse()) {
        for (const auto& row : *data) {
            double principal = std::stod(row[0]);
            int monthTaken = std::stoi(row[1]);
            double rate = std::stod(row[2]);
            double minimumMonthlyPayment = std::stod(row[3]);
            std::string id = row[4];
            DEBUG_PRINT("{:.2f} {:.2f} {}", principal, rate, id);
            masterDebt.emplace_back(principal, rate, Debt::PERIOD_YEARLY, id, minimumMonthlyPayment, monthTaken);
        }
    }

    Worker::setMasterDebt(masterDebt);

    for (auto& d : masterDebt) {
        convertToMonthly(d);
    }

    std::ranges::sort(masterDebt, std::ranges::greater(), &Debt::rate);

    simulations.open("simulations.csv", std::ios_base::binary);
    for (unsigned int i = 0; i < numWorkers; i++) {
        workers.emplace_back(ITERATIONS / numWorkers, i);
    }
    for (auto& w : workers) {
        w.start();
    }
    for (auto& w : workers) {
        w.join();
    }
    for (unsigned int i = 0; i < numWorkers; i++) {
        std::string partialSim = FILE_PREFIX + std::to_string(i) + ".csv";
        std::ifstream input{partialSim, std::ios_base::binary};
        simulations << input.rdbuf();
        input.close();
        std::filesystem::remove(std::string(partialSim));
    }

    simulations.close();

    return 0;
}
