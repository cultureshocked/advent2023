#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <unordered_map>

int64_t challenge_one(std::fstream& fs);
int64_t challenge_two(std::fstream& fs);

int main(void) {
  std::fstream fs("input.txt");
  if (!fs.is_open())
    return -1;

  std::cout << "Challenge one: " << challenge_one(fs) << std::endl;

  fs.close();
  return 0;
}

int64_t challenge_one(std::fstream& fs) {
  std::vector<std::string> lines {};
  std::string line;
  while (std::getline(fs, line)) {
    lines.push_back(line);
  }

  std::string directions {lines[0]};

  std::vector<std::pair<std::string, std::string>> options {};
  std::unordered_map<std::string , int64_t> lookup {};
  //ignore first two lines
  for (int i = 2; i < lines.size(); ++i) {
    lookup[lines[i].substr(0, 3)] = i - 2;
    options.push_back(std::pair {lines[i].substr(7, 3), lines[i].substr(12, 3)});
  }

  int64_t curr_loc {lookup["AAA"]};
  int64_t dest_loc {lookup["ZZZ"]};
  int64_t total_steps {0};
  while (curr_loc != dest_loc) {
    std::string selected_route {((directions[total_steps % directions.length()] == 'L') ? options[curr_loc].first : options[curr_loc].second)};
    curr_loc = lookup[selected_route];
    total_steps++;
  }
  return total_steps;
}
