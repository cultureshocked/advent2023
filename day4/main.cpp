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

int main(void) {
  std::fstream fs("input.txt");
  if (!fs.is_open())
    return -1;

  std::cout << "Challenge one: " << challenge_one(fs) << "\n";

  fs.clear();
  fs.seekg(0);

  // std::cout << "Challenge two: " << challenge_two(fs) << "\n";

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
  std::vector<int> winning {};
  std::vector<int> actual {};
  std::vector<int> intersection {};
  
  int number;
  std::stringstream ss {};

  ss << card.first; // winning numbers first
  while (ss >> number) {
    winning.push_back(number);
  }
 
  ss.clear(); // clear eof bit
  ss << card.second; // actual numbers second
  while (ss >> number) {
    actual.push_back(number);
  }

  std::ranges::sort(winning);
  std::ranges::sort(actual);

  // to find which numbers are in both sets
  std::ranges::set_intersection(winning, actual, std::back_inserter(intersection));

  //bitshift left is same as doubling
  return (intersection.size() > 0) ? (1 << (intersection.size() - 1)) : 0;
}
