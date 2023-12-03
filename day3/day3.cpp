#include <iostream>
#include <fstream>
#include <algorithm>
#include <numeric>
#include <vector>
#include <cctype>

int challenge_one(std::fstream& fs);
int challenge_two(std::fstream& fs);
std::vector<std::pair<int, int>> generate_adjacent_indeces(int x, int y, int max_x, int max_y);
std::vector<std::pair<int, int>> read_number_space(int x, int y, int max_x, const std::vector<std::string>& in);
int read_number(int x, int y, int length, const std::vector<std::string>& in);

int main(void) {
  std::fstream fs("input.txt");
  if (!fs.is_open())
    return -1;

  std::cout << "Challenge one: " << challenge_one(fs) << std::endl;
  fs.close();

  fs.open("in2.txt");
  if (!fs.is_open())
    return -1;

  std::cout << "Challenge two: " << challenge_two(fs) << std::endl;
  return 0;
}

// i know i should be using auto for long type names, and i do so in the place i use this function,
// but i feel like this should be explicit no?
std::vector<std::pair<int, int>> generate_adjacent_indeces(int x, int y, int max_x, int max_y) {
  // define permutations
  std::vector<std::pair<int, int>> ret {
    {0, 1}, {0, -1}, {1, 0}, {-1, 0}, // up down right left
    {1, 1}, {1, -1}, {-1, 1}, {-1, -1} // se, ne, sw, nw
  };

  // apply premutations to x/y values
  std::for_each(ret.begin(), ret.end(), [&x, &y](std::pair<int, int>& pair){
    pair.first += x;
    pair.second += y;
  });

  // remove out of bounds idxs
  ret.erase(std::remove_if(ret.begin(), ret.end(), [&max_x, &max_y](auto& pair){
    return (pair.first < 0 || pair.first > max_x) || (pair.second < 0 || pair.second > max_y);
  }), ret.end());

  // return
  return ret;
}

std::vector<std::pair<int, int>> read_number_space(int x, int y, int max_x, const std::vector<std::string>& in) {
  std::vector<std::pair<int, int>> ret {};
  int offset {0};

  //read left
  while (x + offset >= 0 && std::isdigit(in[y][x+offset])) {
    ret.push_back({x+offset, y});
    offset -= 1;
  }

  //read right
  offset = 1;
  while (x + offset <= max_x && std::isdigit(in[y][x+offset])) {
    ret.push_back({x+offset, y});
    offset += 1;
  }

  // sort (prepare for std::set_difference) + return
  std::sort(ret.begin(), ret.end());
  return ret;
}

int read_number(int x, int length, std::string& line) {
  return std::stod(line.substr(x, length));
}

int challenge_one(std::fstream& fs) {
  std::vector<std::string> lines {};
  std::vector<int> part_numbers {0};
  std::string line {};
  while (std::getline(fs, line)) {
    lines.push_back(line);
  }

  // we need to see the data in 2d space, so we need to use indexing/randomaccess
  // PRECONDITIONS: no numbers are adjacent vertically/diagonally.
  // horizontally adjacent digits are part of same number
  for (int y = 0; y < lines.size(); ++y) {
    for (int x = 0; x < lines[y].length(); ++x) {
      if (std::isdigit(lines[y][x])) {
        std::string number {""};
        int start = x;
        int end = x;
        char ch;

        //scan full number
        while (std::isdigit(ch = lines[y][x]) && x < lines[y].length()) {
          x += 1;
          end += 1;
        }
        number = lines[y].substr(start, end - start);

        // for each char, we want to make sure every adjacent char is
        // either a digit (because adjacent digits dont matter here) or 
        // a . (null value), and if not, then its a valid part
        bool is_valid_part = false;
        for (int idx = start; idx < end; ++idx) {
          // no need to check further
          if (is_valid_part) break;

          // get surrounding coords + check them
          auto adj_coords = generate_adjacent_indeces(idx, y, lines[y].length() - 1, lines.size() - 1);
          is_valid_part = ! std::all_of(adj_coords.cbegin(), adj_coords.cend(), [&lines](std::pair<int, int> adj){
            return lines[adj.second][adj.first] == '.' || std::isdigit(lines[adj.second][adj.first]);
          });
        }
        part_numbers.push_back((is_valid_part) ? std::stod(number) : 0);
      }
    }
  }
  return std::accumulate(part_numbers.begin(), part_numbers.end(), 0);
}

int challenge_two(std::fstream& fs) {
  std::vector<std::string> lines {};
  std::string line {""};
  std::vector<int> gear_ratios {};

  while (std::getline(fs, line)) {
    lines.push_back(line);
  }

  // PRECONDITIONS FOR GEARS:
  // there may be 0, 1, or 2 numbers adjacent. No more.
  // we are going to validate against the constraint though in case im wrong
  for (int y = 0; y < lines.size(); ++y) {
    for (int x = 0; x < lines[y].length(); ++x) {
      //found gear
      if (lines[y][x] == '*') {
        //since we are using std::set_difference, we will need to sort vectors too
        std::vector<int> adjacent_numbers {};
        auto adjacent_squares = generate_adjacent_indeces(x, y, lines[y].length() - 1, lines.size() - 1);
        std::sort(adjacent_squares.begin(), adjacent_squares.end());
        while (adjacent_squares.size()) {
          // start with the last one; no particular reason except .pop() is a fast operation for std::vector
          // also it ensures the order will remain sorted.
          auto pos = adjacent_squares.back();
          if (std::isdigit(lines[pos.second][pos.first])) {
            auto num_positions = read_number_space(pos.first, pos.second, lines[pos.second].length(), lines);
            // this is ugly and im sorry
            adjacent_numbers.push_back(
              std::stod(
                lines[pos.second].substr(
                  num_positions[0].first, 
                  num_positions.back().first - num_positions[0].first + 1
                )
              )
            );

            // remove this number's indeces from the list of indeces to check for numbers
            std::vector<std::pair<int, int>> diff {};
            std::set_difference(adjacent_squares.begin(), adjacent_squares.end(), num_positions.begin(), num_positions.end(), std::inserter(diff, diff.begin()));
            std::sort(diff.begin(), diff.end());
            adjacent_squares = diff;
          }

          if (pos == adjacent_squares.back())
            adjacent_squares.pop_back();
        }
        
        //if there are exactly two gears, we append the ratio
        if (adjacent_numbers.size() == 2) {
          gear_ratios.push_back(
            adjacent_numbers[0] * adjacent_numbers[1]
          );
        }
      }
    }
  }

  return std::accumulate(gear_ratios.begin(), gear_ratios.end(), 0);
}
