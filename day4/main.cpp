#include <iostream>
#include <fstream>
#include <algorithm>
#include <numeric>
#include <vector>
#include <sstream>

int challenge_one(std::fstream& fs);
int challenge_two(std::fstream& fs);

using Card = std::pair<std::string, std::string>;

int calculate_points(Card card);
int matching_numbers(Card card);

int main(void) {
  std::fstream fs("input.txt");
  if (!fs.is_open())
    return -1;

  std::cout << "Challenge one: " << challenge_one(fs) << "\n";

  fs.clear();
  fs.seekg(0);

  std::cout << "Challenge two: " << challenge_two(fs) << "\n";

  fs.close();
  return 0;
}

int challenge_one(std::fstream& fs) {
  std::vector<std::string> lines {};
  std::vector<int> points {};
  std::string line;

  while (std::getline(fs, line))
    lines.push_back(line);
  
  // trim off card ID prologue
  std::ranges::for_each(lines, [](std::string& str){
    str = str.substr(str.find(':') + 2);
  });

  // splits winning numbers from actual numbers
  std::vector<Card> cards {};
  std::transform(lines.begin(), lines.end(), std::back_inserter(cards), [](std::string& str){
    return Card {str.substr(0, str.find('|') - 1), str.substr(str.find('|') + 2)};
  });

  // calculates the points of each card
  std::ranges::transform(cards, std::back_inserter(points), [](Card card){
    return calculate_points(card);
  });

  //sums up the points
  return std::accumulate(points.begin(), points.end(), 0);
}

int calculate_points(Card card) {
  int matching {matching_numbers(card)};

  //bitshift left is same as doubling
  return (matching > 0) ? (1 << (matching - 1)) : 0;
}

int challenge_two(std::fstream& fs) {
  std::vector<std::string> lines {};
  std::string line {};
  
  while (std::getline(fs, line)) 
    lines.push_back(line.substr(line.find(':') + 2));

  std::vector<Card> card_data {};
  std::ranges::transform(lines, std::back_inserter(card_data), [](std::string& str){
    return Card{str.substr(0, str.find('|') - 1), str.substr(str.find('|') + 2)};
  });
  
  std::vector<int> scratchcards(lines.size(), 1);

  for (int i = 0; i < scratchcards.size(); ++i) {
    int num_cards {scratchcards[i]};
    int win_count {matching_numbers(card_data[i])};
    for (int j = i + 1; j < i + win_count + 1 && j < scratchcards.size(); ++j) {
      scratchcards[j] += num_cards;
    }
  }

  return std::accumulate(scratchcards.begin(), scratchcards.end(), 0);
}

int matching_numbers(Card card) {
  std::vector<int> winning {};
  std::vector<int> actual {};
  std::vector<int> intersection {};

  std::stringstream ss;
  int num {0};

  ss << card.first; //winning numbers first
  while (ss >> num)
    winning.push_back(num);

  ss.clear();
  ss << card.second;
  while (ss >> num)
    actual.push_back(num);

  std::ranges::sort(winning);
  std::ranges::sort(actual);

  std::ranges::set_intersection(winning, actual, std::back_inserter(intersection));

  return intersection.size();
}
