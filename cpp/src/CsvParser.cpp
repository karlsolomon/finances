/**
 * @file CsvParser.hpp
 * @brief Defines a class for parsing CSV files into a 2D vector of strings.
 */

#include "CsvParser.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

[[nodiscard]] auto CsvParser::parse() const -> std::optional<std::vector<std::vector<std::string>>> {
    std::ifstream file(file_path);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file: " << file_path << '\n';
        return std::nullopt;
    }

    std::vector<std::vector<std::string>> data;
    std::string line;

    while (std::getline(file, line)) {
        std::istringstream line_stream(line);
        std::vector<std::string> row;
        std::string cell;

        while (std::getline(line_stream, cell, delimiter)) {
            row.push_back(cell);
        }

        data.push_back(std::move(row));
    }

    return data;
};
