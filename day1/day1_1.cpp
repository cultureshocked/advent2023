#include <iostream>
#include <fstream>
#include <algorithm>
#include <numeric>
#include <vector>
#include <cctype>
#include <cstdio>

int main(void) {
  std::vector<std::string> input_lines;
  std::ifstream fp {std::ifstream("input1_1.txt")};
  if (!fp.is_open())
    return -1;
  
  std::string line;
  while (std::getline(fp, line)) {
    input_lines.push_back(line);
  }

  fp.close();

  std::for_each(input_lines.begin(), input_lines.end(), [](std::string& str) {
    auto first = std::find_if(str.cbegin(), str.cend(), static_cast<int(*)(int)>(std::isdigit));
    auto last = std::find_if(str.rbegin(), str.rend(), static_cast<int(*)(int)>(std::isdigit));
    str = {*first, *last};
  });

  // Is there a better way to accomplish this? (str) -> int
  // vector to vector, some kind of map or smth?
  std::vector<int> input_numbers;
  for (std::string val : input_lines) {
    input_numbers.push_back(std::stoi(val));
  }


  int64_t sum = std::accumulate(input_numbers.cbegin(), input_numbers.cend(), 0);
  std::cout << sum << std::endl;
  return 0;
}
