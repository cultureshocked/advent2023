#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

using Coordinate = std::pair<int, int>;

int64_t challenge_one(std::fstream& fs);
int64_t challenge_two(std::fstream& fs);
Coordinate next_coordinate(std::vector<Coordinate>& visited, Coordinate current_pos, std::vector<std::string>& map);

enum class Dir {
  N, S, E, W
};

int main(void) {
  std::fstream fs("input.txt");
  if (!fs.is_open()) return -1;

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

  int64_t steps {0};
  Coordinate start_pos = {-1, -1}, curr_pos;
  std::vector<Coordinate> visited;
  for (int i = 0; i < lines.size(); ++i) {
    size_t found_s = lines[i].find('S');
    if (found_s != std::string::npos) {
      start_pos = std::pair {found_s, i};
      break;
    }
  }

  curr_pos = start_pos;
  Coordinate next_pos;
  while ((next_pos = next_coordinate(visited, curr_pos, lines)) != start_pos) {
    steps++;
    curr_pos = next_pos;
  }

  return (steps + 1) / 2;
}

int64_t challenge_two(std::fstream& fs) {
  return 0;
}

Coordinate next_coordinate(std::vector<Coordinate>& visited, Coordinate current_pos, std::vector<std::string>& map) {
  std::vector<Coordinate> possible;
  std::vector<Coordinate> permutations {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};
  std::ranges::for_each(permutations, [&current_pos](auto& p){
    p.first += current_pos.first;
    p.second += current_pos.second;
  });

  visited.push_back(current_pos);
  switch (map[current_pos.second][current_pos.first]) {
  case '|':
    possible.push_back(permutations[1]);
    possible.push_back(permutations[3]);
    break;
  case '-':
    possible.push_back(permutations[0]);
    possible.push_back(permutations[2]);
    break;
  case 'F':
    possible.push_back(permutations[1]);
    possible.push_back(permutations[0]);
    break;
  case 'S':
    if (map[permutations[0].second][permutations[0].first] == '-'||map[permutations[0].second][permutations[0].first] == '7'||map[permutations[0].second][permutations[0].first] == 'J')
      return permutations[0];
    if (map[permutations[2].second][permutations[2].first] == '-'||map[permutations[2].second][permutations[2].first] == 'L'||map[permutations[2].second][permutations[2].first] == 'F')
      return permutations[2];
    if (map[permutations[1].second][permutations[1].first] == '|'||map[permutations[1].second][permutations[1].first] == 'J'||map[permutations[1].second][permutations[1].first] == 'L')
      return permutations[1];
    if (map[permutations[3].second][permutations[3].first] == '|'||map[permutations[3].second][permutations[3].first] == '7'||map[permutations[3].second][permutations[3].first] == 'F')
      return permutations[3];
  case 'J':
    possible.push_back(permutations[2]);
    possible.push_back(permutations[3]);
    break;
  case 'L':
    possible.push_back(permutations[0]);
    possible.push_back(permutations[3]);
    break;
  case '7':
    possible.push_back(permutations[1]);
    possible.push_back(permutations[2]);
    break;
  }

  if (std::find(visited.begin(), visited.end(), possible[1]) != visited.end())
    possible.erase(std::remove(possible.begin(), possible.end(), possible[1]), possible.end());

  if (std::find(visited.begin(), visited.end(), possible[0]) != visited.end())
    possible.erase(std::remove(possible.begin(), possible.end(), possible[0]), possible.end());

  if (possible.empty())
    return visited[0];
  return possible[0];
}

