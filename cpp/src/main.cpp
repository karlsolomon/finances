/**
 * @file main.cpp
 * @brief Entry point for the simulation program. Configures workers, parses data, and runs simulations.
 */

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <execution>
#include <filesystem>
#include <format>
#include <fstream>
#include <functional>
#include <iostream>
#include <ostream>
#include <print>
#include <thread>
#include <vector>

#include "../inc/CsvParser.hpp"
#include "../inc/Worker.hpp"
#include "../inc/flags.hpp"

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

auto doWork(int taskID) -> std::pair<double, unsigned int> {
    auto w = Worker(taskID);
    w.run();
    return {w.getTotalPaid(), w.getPeriods()};
}

/**
 * @brief Entry point of the simulation program.
 * Initializes the workers, parses CSV data, and combines simulation results.
    file.close();
 * @return Exit code (0 for success).
 */
auto main() -> int {
    DEBUG_PRINT("creating {} threads", std::thread::hardware_concurrency());

    CsvParser csv("../debt.csv");
    std::ofstream simulations;
    static std::vector<Debt> masterDebt;
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

    for (auto& d : masterDebt) {
        convertToMonthly(d);
    }

    std::ranges::sort(masterDebt, std::ranges::greater(), &Debt::rate);
    Worker::setMasterDebt(masterDebt);

    unsigned int numTasks = ITERATIONS;
    std::vector<int> tasks(numTasks);
    std::vector<std::pair<double, unsigned int>> res = std::vector<std::pair<double, unsigned int>>(numTasks);
    std::iota(tasks.begin(), tasks.end(), 0);
    std::for_each(std::execution::par_unseq, tasks.begin(), tasks.end(),
                  [&res](int taskID) { res[taskID] = doWork(taskID); });

    simulations.open("simulations.csv", std::ios_base::binary);
    for (unsigned int i = 0; i < numTasks; i++) {
        simulations << std::format("{:.2f},{}\n", res[i].first, res[i].second);
    }

    simulations.close();

    return 0;
}
