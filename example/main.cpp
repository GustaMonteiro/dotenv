#include <format>
#include <iostream>

#include "dotenv/dotenv.hpp"

int main() {
    dotenv::load("/Users/ayush272002/Desktop/projects/dotenv/.env");

    // Read variables
    auto port = dotenv::get<int>("PORT");
    auto host = dotenv::get<std::string>("HOST");
    auto debug = dotenv::get<std::string>("DEBUG");
    auto max_conn = dotenv::get<int>("MAX_CONNECTIONS");
    auto timeout = dotenv::get<double>("TIMEOUT");
    auto override_val = dotenv::get<std::string>("OVERRIDE");
    auto empty_val = dotenv::get<std::string>("EMPTY");
    auto spaced_val = dotenv::get<std::string>("SPACED");

    // Print to console
    std::cout << std::format("PORT: {}\n", port.value_or(-1));
    std::cout << std::format("HOST: {}\n", host.value_or("undefined"));
    std::cout << std::format("DEBUG: {}\n", debug.value_or("undefined"));
    std::cout << std::format("MAX_CONNECTIONS: {}\n", max_conn.value_or(-1));
    std::cout << std::format("TIMEOUT: {}\n", timeout.value_or(0.0));
    std::cout << std::format("OVERRIDE: {}\n",
                             override_val.value_or("undefined"));
    std::cout << std::format("EMPTY: '{}'\n", empty_val.value_or("undefined"));
    std::cout << std::format("SPACED: '{}'\n",
                             spaced_val.value_or("undefined"));

    return 0;
}
