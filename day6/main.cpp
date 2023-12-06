#include <iostream>
#include <fstream>
#include <sstream>
#include <numeric>
#include <algorithm>
#include <vector>
#include <cctype>

using RaceData = std::pair<int64_t, int64_t>;

int64_t challenge_one(std::fstream&);
int64_t challenge_two(std::fstream&);

int main(void) {
  std::fstream fs("input.txt");
  if (!fs.is_open())
    return -1;

  std::cout << "Challenge one: " << challenge_one(fs) << std::endl;

  fs.clear();
  fs.seekg(0);
  fs.clear();
  std::cout << "Challenge two: " << challenge_two(fs) << std::endl;
  fs.close();
  return 0;
}

int64_t challenge_one(std::fstream& fs) {
  std::vector<std::string> lines {};
  std::string line;
  std::vector<int64_t> race_durations {};
  std::vector<int64_t> race_records {};
  std::vector<RaceData> races {};
  std::vector<int64_t> ways_to_win {};

  while (std::getline(fs, line))
    lines.push_back(line.substr(line.find(':') + 1));

  std::stringstream ss;
  ss << lines[0];
  int64_t num;
  while (ss >> num){
    race_durations.push_back(num);
  }
  
  ss.clear();
  ss << lines[1];
  while (ss >> num){
    race_records.push_back(num);
  }
  for (int i = 0; i < race_durations.size(); ++i) {
    races.push_back(RaceData {race_durations[i], race_records[i]});
  }

  for (const auto& [d, r] : races) {
    int64_t duration = d;
    int64_t ways = 0;
    while (duration) {
      if (duration * (d - duration) > r)
        ways++;
      duration--;
    }
    ways_to_win.push_back(ways);
  }

  return std::reduce(ways_to_win.begin(), ways_to_win.end(), 1, [](auto a, auto b){
    return a * b;
  });
}

int64_t challenge_two(std::fstream& fs) {
  std::vector<std::string> lines {};
  std::string line;
  RaceData race;
  int64_t ways_to_win {0};
  while (std::getline(fs, line))
    lines.push_back(line.substr(line.find(':') + 1));

  std::ranges::for_each(lines, [](auto& str){
    str.erase(std::remove_if(str.begin(), str.end(), [](auto& ch){
      return std::isspace(ch);
    }), str.end());
  });

  race = RaceData{std::stol(lines[0]), std::stol(lines[1])};
  
  const auto race_duration = race.first;
  const auto race_record = race.second;
  auto current_duration = race.first;
  while (current_duration) {
    if (current_duration * (race_duration - current_duration) > race_record)
      ways_to_win++;
    current_duration--;
  }
  return ways_to_win;
}
