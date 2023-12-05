#include <algorithm>
#include <numeric>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <array>

int challenge_one(std::fstream& fstream);
int challenge_two(std::fstream& fstream);

int64_t map_x_to_y(const int64_t x, const int start, const std::vector<std::string>& map);
int find_first_line_of_table(const int start, const std::vector<std::string>& table);

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

  std::cout << "Seeds: \n";
  for (auto& s : seeds)
    std::cout << s << ", ";
  std::cout << std::endl;

  int start_line {0};
  while ((start_line = find_first_line_of_table(start_line, lines)) != -1) {
    std::ranges::for_each(seeds, [&lines, &start_line](int64_t& s){
      s = map_x_to_y(s, start_line, lines);
    });

    std::cout << "Transformed:\n";
    for (auto& s : seeds) {
      std::cout << s << ", ";
    }
    std::cout << "\n\n";
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

int challenge_two(std::fstream& fs) {
  return 0;
}

int find_first_line_of_table(const int start, const std::vector<std::string>& table) {
  for (int i = start; i < table.size(); ++i) {
    if (table[i][table[i].length() - 1] == ':') return i + 1;
  }
  return -1;
}
