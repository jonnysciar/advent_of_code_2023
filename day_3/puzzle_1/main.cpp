#include <fstream>
#include <algorithm>
#include <vector>
#include <optional>
#include <iostream>
#include <cinttypes>
#include <sstream>

using Position = std::pair<size_t, size_t>;

class Num {
private:
    uint64_t m_value;
    size_t m_num_line;
    Position m_line_position;

public:
    Num(uint64_t value, size_t num_line, Position line_position) : m_value(value), m_num_line(num_line),
                                                                   m_line_position(std::move(line_position)) {
    }

    [[nodiscard]] uint64_t getValue() const {
        return m_value;
    }

    [[nodiscard]] size_t getNumLine() const {
        return m_num_line;
    }

    [[nodiscard]] const Position &getLinePosition() const {
        return m_line_position;
    }

    [[nodiscard]] std::string toString() const {
        std::stringstream ss;
        ss << "Value " << m_value << " in line " << m_num_line << " Pos from " << m_line_position.first << " to "
           << m_line_position.second << '\n';
        return ss.str();
    }

    bool operator==(const Num &rhs) {
        return std::tie(m_value, m_num_line, m_line_position) == std::tie(rhs.m_value, rhs.m_num_line, m_line_position);
    }

    bool operator!=(const Num &rhs) {
        return !(*this == rhs);
    }
};

bool is_num_adj(const std::string &line, const Num &num) {
    const Position &position = num.getLinePosition();
    size_t l_pos = position.first;
    size_t r_pos = position.second;

    if (l_pos > 0) {
        l_pos--;
    }

    if (r_pos < line.size() - 1) {
        r_pos++;
    }

    //std::cout << num.toString();
    //std::cout << "Searching symbol from pos " << l_pos << " to pos " << r_pos << '\n';

    for (size_t i = l_pos; i < r_pos + 1; i++) {
        auto &c = line.at(i);
        if (!std::isdigit(c) && c != '.') {
            //std::cout << "Found symbol '" << c << "' pos " << i << " line "<< line << "\n";
            return true;
        }
    }

    return false;
}

uint64_t is_num_valid(const std::vector<std::string> &lines, const Num &num) {
    size_t num_line = num.getNumLine();

    if (is_num_adj(lines.at(num_line), num)) {
        std::cout << "Num " << num.getValue() << " is valid\n";
        return num.getValue();
    }

    if (num_line > 0 && is_num_adj(lines.at(num_line - 1), num)) {
        std::cout << "Num " << num.getValue() << " is valid\n";
        return num.getValue();
    }

    if (num_line < lines.size() - 1 && is_num_adj(lines.at(num_line + 1), num)) {
        std::cout << "Num " << num.getValue() << " is valid\n";
        return num.getValue();
    }

    return 0;
}

int main() {
    std::ifstream ifs("../input.txt");
    std::string line;
    std::vector<std::string> lines;
    std::vector<Num> nums;
    size_t num_line = 0;
    uint64_t sum = 0;

    while (std::getline(ifs, line)) {
        std::optional<uint64_t> value;
        std::optional<size_t> l_pos;
        std::optional<size_t> r_pos;

        for (size_t i = 0; i < line.size(); i++) {
            const auto &c = line.at(i);
            if (!std::isdigit(c)) {
                if (value) {
                    nums.emplace_back(value.value(), num_line, Position{l_pos.value(), r_pos.value()});
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
            nums.emplace_back(value.value(), num_line, Position{l_pos.value(), r_pos.value()});
        }
        lines.push_back(std::move(line));
        num_line++;
    }

    std::for_each(nums.cbegin(), nums.cend(), [&sum, &lines](const auto &num) {
        sum = sum + is_num_valid(lines, num);
    });

    std::cout << "The sum of parts is " << sum << '\n';
}
