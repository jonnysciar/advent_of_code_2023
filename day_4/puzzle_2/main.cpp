#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <cstdint>
#include <unordered_map>
#include <optional>
#include <unordered_set>

std::unordered_map<uint64_t, uint64_t> num_of_cards;

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

uint64_t parse_line(const std::string &line) {
    auto card_values = tokenize(line, ':');
    uint64_t card_num = std::strtoull(card_values.at(0).substr(5).c_str(), nullptr, 10);
    auto winning_mine = tokenize(card_values.at(1), '|');
    std::unordered_set<std::string> winning_nums;
    uint64_t count = card_num;

    num_of_cards[card_num]++;

    for (const auto &winning_num: tokenize(winning_mine.at(0), ' ')) {
        winning_nums.emplace(winning_num);
    }

    for (const auto &my_num : tokenize(winning_mine.at(1), ' ')) {
        if (winning_nums.count(my_num) == 0) {
            continue;
        }

        num_of_cards[++count] += num_of_cards[card_num];
    }

    std::cout << "Instance of Card " << card_num << " is " << num_of_cards[card_num] << '\n';
    return num_of_cards[card_num];
}


int main() {
    std::ifstream ifs("../input.txt");
    std::string line;
    uint64_t sum = 0;

    while (std::getline(ifs, line)) {
        sum = sum + parse_line(line);
    }

    std::cout << "The sum of games' id is " << sum << '\n';
}
