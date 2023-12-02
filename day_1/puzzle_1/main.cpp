#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <cstdint>

constexpr int32_t char_to_digit(char c) {
    return c - '0';
}

constexpr bool is_digit(char c) {
    int32_t digit = char_to_digit(c);
    return (digit >= 0 && digit <= 9);
}

int main() {
    std::ifstream ifs("../input.txt");
    std::string line;
    uint64_t sum = 0;

    while (std::getline(ifs, line)) {
        int32_t value = 0;
        auto it = std::find_if(line.cbegin(), line.cend(), [](const auto c) { return is_digit(c); });

        if (it == line.cend()) {
            continue;
        }

        value = char_to_digit(*it) * 10;

        auto rev_it = std::find_if(line.rbegin(), line.rend(), [](const auto c) { return is_digit(c); });

        if (rev_it == line.rend()) {
            continue;
        }

        sum = sum + (value + char_to_digit(*rev_it));
    }

    std::cout << "The sum of calibration values is " << sum << '\n';
}
