#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <cstdint>
#include <unordered_map>
#include <optional>

constexpr size_t k_max_size = 5;
const std::unordered_map<std::string, int32_t> str_to_int{
        {"one", 1},
        {"two", 2},
        {"three", 3},
        {"four", 4},
        {"five", 5},
        {"six", 6},
        {"seven", 7},
        {"eight", 8},
        {"nine", 9},
};

std::optional<int32_t> str_digit(const std::string &str) {
    for (const auto &[digit_str, value] : str_to_int) {
        if (digit_str == str.substr(0, digit_str.size())) {
            return value;
        }
    }
    return {};
}

constexpr std::optional<int32_t> char_to_digit(char c) {
    int32_t digit = c - '0';
    if (digit >= 0 && digit <= 9) {
        return digit;
    }

    return {};
}

int main() {
    std::ifstream ifs("../input.txt");
    std::string line;
    uint64_t sum = 0;

    while (std::getline(ifs, line)) {
        std::optional<int32_t> first_digit_opt;
        std::optional<int32_t> last_digit_opt;

        for (int32_t i = 0; i < line.size(); i++) {
            auto c = line[i];
            auto digit_opt = char_to_digit(c);
            if (!digit_opt) {
                digit_opt = str_digit(line.substr(i, k_max_size));
            }

            if (!digit_opt) {
                continue;
            }

            if (!first_digit_opt) {
                first_digit_opt = digit_opt;
            }

            last_digit_opt = digit_opt;
        }

        //std::cout << "first " << first_digit_opt.value() << " second " << last_digit_opt.value() << '\n';
        sum = sum + (first_digit_opt.value_or(0) * 10) + last_digit_opt.value_or(0);
    }

    std::cout << "The sum of calibration values is " << sum << '\n';
}
