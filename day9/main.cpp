#include <iostream>
#include <algorithm>
#include <numeric>
#include <sstream>
#include <fstream>
#include <vector>

using History = std::vector<int64_t>;
using Histories = std::vector<History>;

int64_t challenge_one(std::fstream& fs);
int64_t challenge_two(std::fstream& fs);
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

  std::vector<Histories> hists;
  std::stringstream ss;
  int64_t num {0};
  for (auto& l : lines) {
    Histories h { History {} };
    ss.clear();
    ss << l;
    while (ss >> num)
      h[0].push_back(num);

    hists.push_back(h);
  }

  while (!std::all_of(hists.begin(), hists.end(), [](auto& h){
    return std::all_of(h.back().begin(), h.back().end(), [](auto& x){ return x == 0; }); }
    )) {

    for (auto &h : hists) {
      if (std::all_of(h.back().begin(), h.back().end(),[](auto x) { return x == 0; }))
        continue;

      History new_h {};
      for (int i = 0; i < h.back().size() - 1; ++i) {
        new_h.push_back(h.back()[i+1] - h.back()[i]);
      }

      h.push_back(new_h);
    }
  }

  for (auto& h : hists) {
    h.back().push_back(0L);
    for (int i = h.size() - 2; i >= 0; --i) {
      h[i].push_back(h[i].back() + h[i+1].back());
    }
  }

  int64_t sum {0};
  for (auto& h : hists) {
    sum += h[0].back();
  }

  return sum;
}

int64_t challenge_two(std::fstream& fs) {
  std::vector<std::string> lines;
  std::string line;

  while (std::getline(fs, line))
    lines.push_back(line);

  std::vector<Histories> hists;
  std::stringstream ss;
  int64_t num {0};
  for (auto& l : lines) {
    Histories h { History {} };
    ss.clear();
    ss << l;
    while (ss >> num)
      h[0].push_back(num);

    hists.push_back(h);
  }

  while (!std::all_of(hists.begin(), hists.end(), [](auto& h){
    return std::all_of(h.back().begin(), h.back().end(), [](auto& x){ return x == 0; }); }
                      )) {

    for (auto &h : hists) {
      if (std::all_of(h.back().begin(), h.back().end(),[](auto x) { return x == 0; }))
        continue;

      History new_h {};
      for (int i = 0; i < h.back().size() - 1; ++i) {
        new_h.push_back(h.back()[i+1] - h.back()[i]);
      }

      h.push_back(new_h);
    }
  }

  // this is honestly the only thing that changes. i still end up storing the
  // "previous" value at the end but i do the calculation using the first
  // element in the row still.
  for (auto& h : hists) {
    h.back().push_back(0L);
    for (int i = h.size() - 2; i >= 0; --i) {
      h[i].push_back(h[i][0] - h[i+1].back());
    }
  }

  // since the "previous" value is being stored at the end of the vector
  // this doesnt change
  int64_t sum {0};
  for (auto& h : hists) {
    sum += h[0].back();
  }

  return sum;
}