/**
 * @file flags.hpp
 * @brief Defines global flags and configurations for the application.
 */

#define DEBUG false               ///< Enables debug print statements.
#define KID true                  ///< Specifies if the simulation involves having a child.
#define ITERATIONS (1024 * 1024)  ///< Number of iterations for each core.
#define AGGRESSIVE true           ///< Determines whether put paid off required debts into paying of other debts faster.

// Determines the aggressive payment offset based on the AGGRESSIVE flag.
#if AGGRESSIVE
#define AGGRESSIVE_OFFSET (475.0)  ///< Sum of monthly required payments for aggressive mode.
#else
#define AGGRESSIVE_OFFSET (0.0)  ///< Offset for non-aggressive mode.
#endif

/**
 * @macro DEBUG_PRINT
 * @brief Prints debug information if DEBUG is enabled.
 */
#define DEBUG_PRINT(...) \
    if (DEBUG) std::println(__VA_ARGS__)

#define FILE_PREFIX "sim_"  ///< Prefix for output simulation files.
