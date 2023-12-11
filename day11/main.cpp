#include <iostream>
#include <fstream>
#include <algorithm>
#include <numeric>
#include <vector>
#include <cstdlib>
#include <ranges>

int64_t challenge_one(std::fstream&);
int64_t challenge_two(std::fstream&);
void insert_dot(std::string&, int);

int main(void) {
  std::fstream fs("input.txt");
  if (!fs.is_open())
    return -1;

  std::cout << "Challenge one: " << challenge_one(fs) << std::endl;
  fs.clear();
  fs.seekg(0);
  
  std::cout << "Challenge two: " << challenge_two(fs) << std::endl;
  fs.close();
  return 0;
}

int64_t challenge_one(std::fstream& fs) {
  std::vector<std::string> lines;
  std::string line;
  while (std::getline(fs, line))
    lines.push_back(line);

  //horizontal lines distort
  int64_t i {0};
  auto iter {lines.begin()};
  while (i < lines.size()) {
    if (std::all_of(lines[i].begin(), lines[i].end(), [](auto ch){ return ch == '.'; })) {
      lines.insert(iter, std::string(lines[i].length(), '.'));
      i++;
      iter++;
    }
    i++;
    iter++;
  }

  //vertical lines distort
  i = 0;
  while (i < lines[0].length()) {
    if (std::all_of(lines.begin(), lines.end(), [&i](auto str){
      return str[i] == '.';
    })) {
      std::for_each(lines.begin(), lines.end(), [&i](auto& str){
        insert_dot(str, i);
      });
      i++;
    }
    i++;
  }

  std::vector<std::pair<int, int>> galaxies;
  for (i = 0; i < lines.size(); ++i) {
    for (int j = 0; j < lines[i].length(); ++j) {
      if (lines[i][j] == '#')
        galaxies.push_back(std::pair { j, i });
    }
  }

  std::vector<int> shortest_distances;
  for (i = 0; i < galaxies.size(); ++i) {
    for (int j = i + 1; j < galaxies.size(); ++j) {
      shortest_distances.push_back(
        std::abs(galaxies[i].first - galaxies[j].first) + std::abs(galaxies[i].second - galaxies[j].second)
      );
    }
  }
  return std::accumulate(shortest_distances.begin(), shortest_distances.end(), 0);
}

int64_t challenge_two(std::fstream& fs) {
  std::vector<std::string> lines;
  std::string line;
  while (std::getline(fs, line))
    lines.push_back(line);

  std::vector<int> distorted_rows;
  std::vector<int> distorted_cols;
  for (auto [i, str] : std::views::enumerate(lines)) {
    if (std::ranges::all_of(str, [](auto ch){return ch == '.';})) {
      distorted_rows.push_back(i);
    }
  }

  for (int i = 0; i < lines[0].length(); ++i) {
    if (std::ranges::all_of(lines, [&i](auto str){ return str[i] == '.'; }))
      distorted_cols.push_back(i);
  }

  std::vector<std::pair<int, int>> galaxies;
  for (int i = 0; i < lines.size(); ++i) {
    for (int j = 0; j < lines[i].length(); ++j) {
      if (lines[i][j] == '#')
        galaxies.push_back(std::pair { j, i });
    }
  }

  std::vector<int64_t> shortest_distances;
  for (int i = 0; i < galaxies.size(); ++i) {
    for (int j = i + 1; j < galaxies.size(); ++j) {
      int64_t dist {0};
      int min_x = std::min(galaxies[i].first, galaxies[j].first);
      int max_x = std::max(galaxies[i].first, galaxies[j].first);
      int min_y = std::min(galaxies[i].second, galaxies[j].second);
      int max_y = std::max(galaxies[i].second, galaxies[j].second);
      for (auto n : distorted_cols) {
        if (min_x < n && n < max_x)
          dist += 999'999;
      }
      for (auto n : distorted_rows) {
        if (min_y < n && n < max_y)
          dist += 999'999;
      }
      shortest_distances.push_back(
          (max_x - min_x) + (max_y - min_y) + dist
      );
    }
  }

  return std::accumulate(shortest_distances.begin(), shortest_distances.end(), 0L);
}

void insert_dot(std::string& str, int pos) {
  str = str.substr(0, pos) + std::string(".") + str.substr(pos);
}
