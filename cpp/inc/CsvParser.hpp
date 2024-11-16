/**
 * @file CsvParser.hpp
 * @brief Defines a class for parsing CSV files into a 2D vector of strings.
 */

#include <optional>
#include <string>
#include <utility>
#include <vector>

/**
 * @class CsvParser
 * @brief A utility class to parse CSV files into a structured format.
 */
class CsvParser {
 private:
    std::string file_path;  ///< The path to the CSV file to be parsed.
    char delimiter;         ///< The delimiter used in the CSV file (default is ',').

 public:
    /**
     * @brief Constructs a CsvParser object.
     * @param file_path The path to the CSV file.
     * @param delimiter The delimiter used in the file (default is ',').
     */
    CsvParser(std::string file_path, char delimiter = ',') : file_path(std::move(file_path)), delimiter(delimiter) {}

    /**
     * @brief Parses the entire CSV file into a 2D vector of strings.
     * @return A 2D vector containing rows and cells, or std::nullopt if the file cannot be opened.
     */
    [[nodiscard]] auto parse() const -> std::optional<std::vector<std::vector<std::string>>>;
};

// USAGE
/*
int main() {
    CsvParser parser("../debt.csv");

    // Parsing the CSV file using the parse method
    if (auto data = parser.parse()) {
        std::cout << "Parsed using parse():\n";
        for (const auto& row : *data) {
            for (const auto& cell : row) {
                std::cout << cell << " ";
            }
            std::cout << '\n';
        }
    }
    return 0;
}
*/
