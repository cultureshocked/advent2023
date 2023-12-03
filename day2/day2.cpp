#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <sstream>

int challenge_one(std::ifstream& fs);
int challenge_two(std::ifstream& fs);

    // are symbolic constants preferred in global scope like preprocessor #defines?
// the book im reading mentioned smth about using constexpr definitions over
// preprocessor directives. im keeping them in global scope because
// c preprocessor defines are generally at the top of the file for consts.
constexpr int num_red   = 12;
constexpr int num_green = 13;
constexpr int num_blue  = 14;

// i got this one from stack overflow im gonna admit
// enables me to switch{} on std::string.c_str()
/*constexpr unsigned int hash(const char *s, int off = 0) {
    return !s[off] ? 5381 : (hash(s, off+1)*33) ^ s[off];                           
}*/

int main(void) {
  std::ifstream fs {std::ifstream("input.txt")};
  if (!fs.is_open())
    return -1;

  std::cout << "Challenge one: " << challenge_one(fs) << std::endl;

  fs.close();

  fs.open("input.txt");
  if (!fs.is_open())
    return -1;

  std::cout << "Challenge two: " << challenge_two(fs) << std::endl;
  fs.close();
  return 0;

  /*std::vector<int> game_ids {};
  std::vector<int> powers {};
  std::string line;
  int id = 1;

  while (std::getline(fs, line)) {
    int min_red = 0;
    int min_green = 0;
    int min_blue = 0;
    line += ';'; //needs to end with a semicolon
    std::vector<std::string> hands {};
    line = line.substr(line.find(':') + 1); // trims-left of ID and filler.

    // split at each semicolon
    int delimit_idx;
    while ((delimit_idx = line.find(';')) != std::string::npos) {
      // appending an extra comma for delimiting each cube count later
      // preserving leading space for same reason
      hands.push_back(line.substr(0, delimit_idx) + ",");
      line = line.substr(delimit_idx + 1);
    }

    bool is_valid = true;
    for (auto& hand : hands) {
      std::string cubes {};
      while ((delimit_idx = hand.find(',')) != std::string::npos) {
        cubes = hand.substr(1, delimit_idx - 1);

        int num = stoi(cubes.substr(0, cubes.find(' '))); // number
        std::string color = cubes.substr(cubes.find(' ') + 1); // color
        switch (hash(color.c_str())) {
          case hash("red"):
            is_valid = num <= num_red;
            min_red = (min_red < num) ? num : min_red;
            break;
          case hash("green"):
            is_valid = num <= num_green;
            min_green = (min_green < num) ? num : min_green;
            break;
          case hash("blue"):
            is_valid = num <= num_blue;
            min_blue = (min_blue < num) ? num : min_blue;
            break;
          default:
            std::cerr << "ERR: Invalid color detected" << std::endl;
            fs.close();
            return -1;
        }

        hand = (delimit_idx == hand.length() - 1) ? "" : hand.substr(delimit_idx + 1);
      }
    }
    powers.push_back(min_red * min_green * min_blue);
    game_ids.push_back(((is_valid) ? id : 0));
    ++id;
  }

  auto sum_of_ids = std::accumulate(game_ids.begin(), game_ids.end(), 0);
  auto sum_of_powers = std::accumulate(powers.begin(), powers.end(), 0);
  std::cout << "Challenge 1: " << sum_of_ids << std::endl;
  std::cout << "Challenge 2: " << sum_of_powers << std::endl;
  fs.close();
   */

}

int challenge_one(std::ifstream& fs) {
  std::vector<int> ids {};
  std::string line;

  int id = 1;
  while (std::getline(fs, line)) {
    bool is_valid;

    // trim-left game id
    std::vector<std::string> hands {};
    line += ';';
    line = line.substr(line.find(':') + 1);

    //split hands in each game
    int delimiter_idx;
    while ((delimiter_idx = line.find(';')) != std::string::npos) {
      hands.push_back(line.substr(0, delimiter_idx) + ",");
      line = line.substr(delimiter_idx + 1);
    }

    //split hands into values + validate them
    for (auto& hand : hands) {
      is_valid = true;
      std::vector<std::string> cubes {};

      // get each cube data per hand
      while ((delimiter_idx = hand.find(',')) != std::string::npos) {
        cubes.push_back(hand.substr(0, delimiter_idx));
        hand = hand.substr(delimiter_idx + 1);
      }

      //validate hand
      is_valid = std::all_of(cubes.begin(), cubes.end(), [](std::string cube_data) {
        std::stringstream ss;
        std::string color;
        int count;

        ss << cube_data;
        ss >> count >> color;

        switch (color[0]) {
        case 'r':
          return count <= num_red;
        case 'g':
          return count <= num_green;
        case 'b':
          return count <= num_blue;
        default:
          return false;
        }
      });
      if (!is_valid) break;

    }

    ids.push_back((is_valid) ? id : 0);
    ++id;
  }
  return std::accumulate(ids.begin(), ids.end(), 0);
}

int challenge_two(std::ifstream& fs) {
  std::vector<int> powers {};
  std::string line;

  while (std::getline(fs, line)) {
    int min_red = 0;
    int min_green = 0;
    int min_blue = 0;

    // trim-left game id
    std::vector<std::string> hands {};
    line += ';';
    line = line.substr(line.find(':') + 1);

    //split hands in each game
    int delimiter_idx;
    while ((delimiter_idx = line.find(';')) != std::string::npos) {
      hands.push_back(line.substr(0, delimiter_idx) + ",");
      line = line.substr(delimiter_idx + 1);
    }

    //split hands into values + validate them
    for (auto& hand : hands) {
      std::vector<std::string> cubes {};

      // get each cube data per hand
      while ((delimiter_idx = hand.find(',')) != std::string::npos) {
        cubes.push_back(hand.substr(0, delimiter_idx));
        hand = hand.substr(delimiter_idx + 1);
      }

      //check if hand exceeds min game requirements for each cube type
      std::for_each(cubes.begin(), cubes.end(), [&min_red, &min_green, &min_blue](std::string cube_data) {
        std::stringstream ss;
        std::string color;
        int count;

        ss << cube_data;
        ss >> count >> color;

        switch (color[0]) {
        case 'r':
          min_red = (min_red < count) ? count : min_red;
          break;
        case 'g':
          min_green = (min_green < count) ? count : min_green;
          break;
        case 'b':
          min_blue = (min_blue < count) ? count : min_blue;
          break;
        }
      });
    }
    powers.push_back(min_red * min_green * min_blue);
  }

  return std::accumulate(powers.begin(), powers.end(), 0);
}