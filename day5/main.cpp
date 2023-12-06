#include <algorithm>
#include <numeric>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <array>
#include <cstdint>

using Table = std::vector<std::string>;

int64_t challenge_one(std::fstream& fstream);
int64_t challenge_two(std::fstream& fstream);

int64_t map_x_to_y(const int64_t x, const int start, const std::vector<std::string>& map);
int64_t lookup(const std::vector<Table>& maps, const int64_t n);
int find_first_line_of_table(const int start, const std::vector<std::string>& table);
Table extract_table(const Table tb, const int start);

int main(void) {
  std::fstream fs("input.txt");
  if (!fs.is_open())
    return -1;

  std::cout << "Challenge one: " << challenge_one(fs) << "\n";
  fs.clear();
  fs.seekg(0);
  std::cout << "Challenge two: ";
  std::cout << challenge_two(fs) << "\n";
  fs.close();
  return 0;
}

int64_t challenge_one(std::fstream& fs) {
  std::vector<int64_t> seeds;
  std::vector<std::string> lines {};
  std::string line {};

  while (std::getline(fs, line)) {
    lines.push_back(line);
  }

  lines[0] = lines[0].substr(lines[0].find(':') + 1);

  std::stringstream ss;
  ss << lines[0];
  int64_t seed {};
  while (ss >> seed) {
    seeds.push_back(seed);
  }


  int start_line {0};
  while ((start_line = find_first_line_of_table(start_line, lines)) != -1) {
    std::ranges::for_each(seeds, [&lines, &start_line](int64_t& s){
      s = map_x_to_y(s, start_line, lines);
    });

  }
  
  return std::ranges::min(seeds);
}

int64_t map_x_to_y(const int64_t x, const int map_start, const std::vector<std::string>& map) {
  int current_line {map_start};
  int64_t res {-1};
  for (int i = map_start; i < map.size(); ++i) {
    if (map[i] == "") break;
    if (res >= 0) break;
    std::string data = map[i];
    {
      std::array<int64_t,3> arr {0,0,0};
      std::stringstream ss;
      ss << data;
      ss >> arr[0];
      ss >> arr[1];
      ss >> arr[2];

      if (x >= arr[1] && x <= arr[1] + arr[2]) {
        res = arr[0] + (x - arr[1]);
      }
    }
  }

  return (res > 0) ? res : x;
}

int64_t lookup(const std::vector<Table>& maps, const int64_t n) {
  int64_t counter {n};
  for (auto& table : maps) {
    counter = map_x_to_y(counter, 0, table);
  }
  return counter;
}

int64_t challenge_two(std::fstream& fs) {
  std::vector<std::string> lines {};
  std::vector<int64_t> seeds {};
  std::string line {};

  while (std::getline(fs, line))
    lines.push_back(line);

  lines[0] = lines[0].substr(lines[0].find(':') + 1);

  std::vector<int64_t> seed_numbers {};
  std::stringstream ss;
  ss << lines[0];
  int64_t num {};
  while (ss >> num)
    seed_numbers.push_back(num);

  std::vector<std::pair<int64_t, int64_t>> seed_ranges {};
  for (int i = 0; i < seed_numbers.size(); i+=2) 
    seed_ranges.push_back(std::pair(seed_numbers[i], seed_numbers[i+1]));

  std::vector<std::pair<int64_t, int64_t>> seed_minmax {};
  std::ranges::transform(seed_ranges, back_inserter(seed_minmax), [](auto& range){
    return std::pair {range.first, range.first + range.second};
  });
  
  std::vector<Table> tables {};
  uint64_t start_line {0};
  while ((start_line = find_first_line_of_table(start_line, lines)) != -1) {
    tables.push_back(extract_table(lines, start_line));
  }

  std::vector<std::vector<std::tuple<int64_t, int64_t, int64_t>>> blocks {};
  std::ranges::transform(tables, std::back_inserter(blocks), [](auto& tb){
    std::stringstream ss;
    std::vector<std::tuple<int64_t, int64_t, int64_t>> ret {};
    for (auto& line : tb) {
      int64_t a, b, c;
      ss.clear();
      ss << line;
      ss >> a;
      ss >> b;
      ss >> c;
      ret.push_back(std::tuple {a, b, c});
    }
    return ret;
  });

  std::ranges::sort(seed_minmax);

  for (auto& block : blocks) {

    std::vector<std::pair<int64_t, int64_t>> new_seeds {};
    while (seed_minmax.size()) {
      auto current_range {seed_minmax[seed_minmax.size() - 1]};
      auto s = current_range.first, e = current_range.second;
      seed_minmax.pop_back();
      bool range_sentinel = false;
      for (auto& [a, b, c] : block) {
        int64_t os {(s > b) ? s : b};
        int64_t oe {(e < b + c) ? e : b + c};

        if (os < oe) {
          new_seeds.push_back(std::pair {os - b + a, oe - b + a});

          if (os > s)
            seed_minmax.push_back(std::pair {s, os});
          if (e > oe)
            seed_minmax.push_back(std::pair {oe, e});

          range_sentinel = true;
          break;
        }
      } 
      if (!range_sentinel)
        new_seeds.push_back(std::pair {s, e});
      
    }
    seed_minmax = new_seeds;
  }
  std::ranges::sort(seed_minmax);

  return seed_minmax[0].first;
}

std::vector<std::string> extract_table(const std::vector<std::string> data, const int start) {
  std::vector<std::string> ret {};
  for (int i = start; i < data.size(); ++i) {
    if (data[i] == "") break;
    ret.push_back(data[i]);
  }
  return ret;
}

int find_first_line_of_table(const int start, const std::vector<std::string>& table) {
  for (int i = start; i < table.size(); ++i) {
    if (table[i][table[i].length() - 1] == ':') return i + 1;
  }
  return -1;
}

