#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <cstdint>
#include <unordered_map>
#include <optional>

const std::unordered_map<std::string, int32_t> max_values_map = {{"red",   12},
                                                                 {"green", 13},
                                                                 {"blue",  14}};

int32_t get_game_id(const std::string &line) {
    std::string id_str = line.substr(5);
    return std::stoi(id_str, nullptr, 10);
}

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
    auto first_split = tokenize(line, ':');
    int32_t game_id = get_game_id(first_split.at(0));
    auto extractions = tokenize(first_split.at(1), ';');

    for (const auto &extraction: extractions) {
        auto values = tokenize(extraction, ',');
        if (std::any_of(values.cbegin(), values.cend(), [](const auto &value) {
            auto value_color = tokenize(value, ' ');
            int32_t num = std::stoi(value_color.at(0));
            return num > max_values_map.at(value_color.at(1));
        })) {
            return 0;
        }
    }

    return game_id;
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
