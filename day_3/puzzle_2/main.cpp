#include <fstream>
#include <algorithm>
#include <vector>
#include <optional>
#include <iostream>
#include <cinttypes>
#include <sstream>

struct Position {
    size_t line;
    size_t l_pos;
    size_t r_pos;

    [[nodiscard]] std::string toString() const {
        std::stringstream ss;
        ss << "line " << line << " pos from " << l_pos << " to "
           << r_pos << '\n';
        return ss.str();
    }
};

struct Num {
    uint64_t value;
    Position position;

    [[nodiscard]] std::string toString() const {
        std::stringstream ss;
        ss << "Value " << value << " in " << position.toString();
        return ss.str();
    }
};

bool is_num_adj(const Position &gear, const Num &num) {
    if (num.position.line > gear.line + 1 || (gear.line > 0 && num.position.line < gear.line - 1)) {
        return false;
    }

    size_t l_pos = num.position.l_pos;
    size_t r_pos = num.position.r_pos + 1;

    if (l_pos > 0) l_pos--;

    std::cout << num.toString() << " is valid " << std::boolalpha << (gear.l_pos >= l_pos && gear.r_pos <= r_pos) << '\n';
    return gear.l_pos >= l_pos && gear.r_pos <= r_pos;
}

uint64_t is_gear_valid(const Position &gear, const std::vector<Num> &nums) {
    size_t count = 0;
    uint64_t power = 1;

    std::for_each(nums.cbegin(), nums.cend(), [&](const auto &num) {if (is_num_adj(gear, num)) {
        count++;
        power *= num.value;
    }});

    if (count != 2) return 0;

    return power;
}

int main() {
    std::ifstream ifs("../input.txt");
    std::string line;
    std::vector<Num> nums;
    std::vector<Position> gears;
    size_t num_line = 0;
    uint64_t sum = 0;

    while (std::getline(ifs, line)) {
        std::optional<uint64_t> value;
        std::optional<size_t> l_pos;
        std::optional<size_t> r_pos;

        for (size_t i = 0; i < line.size(); i++) {
            const auto &c = line.at(i);
            if (!std::isdigit(c)) {
                if (c == '*') {
                    gears.emplace_back(Position{num_line, i, i});
                }
                if (value) {
                    nums.emplace_back(Num{value.value(), Position{num_line, l_pos.value(), r_pos.value()}});
                    value.reset();
                    l_pos.reset();
                    r_pos.reset();
                }
                continue;
            }

            value = (value.value_or(0) * 10) + (c - '0');
            if (!l_pos) {
                l_pos = i;
            }

            if (!r_pos || i > r_pos.value()) {
                r_pos = i;
            }
        }
        if (value) {
            nums.emplace_back(Num{value.value(), Position{num_line, l_pos.value(), r_pos.value()}});
        }
        num_line++;
    }

    std::for_each(gears.cbegin(), gears.cend(), [&sum, &nums](const auto &gear) {
        std::cout << "Check gear in pos " << gear.toString();
        sum += + is_gear_valid(gear, nums);
    });

    std::cout << "The sum of parts is " << sum << '\n';
}
