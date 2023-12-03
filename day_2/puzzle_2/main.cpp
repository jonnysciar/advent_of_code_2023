#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <cstdint>
#include <unordered_map>
#include <optional>

std::vector<std::string> tokenize(std::string str, char delimiter) {
    std::vector<std::string> vec;
    while (!str.empty()) {
        size_t delimiter_pos = str.find(delimiter);
        if (delimiter_pos == std::string::npos) {
            vec.push_back(str);
            break;
        }
        std::string substr = str.substr(0, delimiter_pos);
        if (!substr.empty()) {
            vec.push_back(std::move(substr));
        }
        str = str.substr(delimiter_pos + 1);
    }

    return vec;
}

int32_t parse_line(const std::string &line) {
    int32_t power = 1;
    auto first_split = tokenize(line, ':');
    auto extractions = tokenize(first_split.at(1), ';');
    std::unordered_map<std::string, int32_t> min_values{{"red",   1},
                                                        {"green", 1},
                                                        {"blue",  1}};

    for (const auto &extraction: extractions) {
        auto values = tokenize(extraction, ',');

        for (const auto &value: values) {
            auto colors_value = tokenize(value, ' ');
            int32_t color_value = std::stoi(colors_value.at(0));
            if (color_value > min_values.at(colors_value.at(1))) {
                min_values.at(colors_value.at(1)) = color_value;
            }
        }
    }

    std::for_each(min_values.cbegin(), min_values.cend(), [&power](const auto &min_value) {
        power = power * min_value.second;
    });

    return power;
}


int main() {
    std::ifstream ifs("../input.txt");
    std::string line;
    int32_t sum = 0;

    while (std::getline(ifs, line)) {
        sum = sum + parse_line(line);
    }

    std::cout << "The sum of games' id is " << sum << '\n';
}
