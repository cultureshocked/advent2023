#include <iostream>
#include <numeric>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <vector>
#include <functional>
#include <cctype>
#include <utility>

int64_t challenge_one(std::fstream& fs);
int64_t challenge_two(std::fstream& fs);

enum class CardValue {
  TWO = 2,
  THREE,
  FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN, JACK, QUEEN, KING, ACE
};

enum class StrongOrder {
  HIGHCARD, TWO_OF_KIND, TWO_PAIR, THREE_OF_KIND, FULL_HOUSE, FOUR_OF_KIND, FIVE_OF_KIND, ERR
};

struct Hand {
  std::vector<CardValue> h {};
  std::string so {};
  bool operator<(const Hand& other) { 
    if (other.so == so) {
      for (int i = 0; i < h.size(); ++i) {
        if (other.h[i] != h[i]) return h[i] < other.h[i];
      }
    }
    for (int i = 0; i < so.length(); ++i) {
      if (so[i] != other.so[i]) return so[i] < other.so[i];
    }
    return false;
  }
  bool operator>(const Hand& other) {
    if (other.so == so) {
      for (int i = 0; i < h.size(); ++i) {
        if (other.h[i] != h[i]) return h[i] > other.h[i];
      }
    }
    for (int i = 0; i < so.size(); ++i) {
      if (other.so[i] != so[i]) return so[i] > other.so[i];
    }
    return false;
  }
  bool operator==(const Hand& other) {
    return h == other.h;
  }

  bool operator<=(const Hand& other) {
    return operator<(other);
  }

  bool operator>=(const Hand& other) {
    return operator>(other);
  }

  void generate_strongorder() {
    
    std::vector<CardValue> localhand {h};
    std::vector<int> cardcounts {};
    std::ranges::sort(localhand);
    
    std::cout << std::endl;
    int idx {0};
    while (idx < localhand.size()) {
      int64_t element_count {std::count(localhand.begin(), localhand.end(), localhand[idx])};
      idx += element_count;
      cardcounts.push_back(element_count + 0x30);
      
    }
    
    for (auto val : cardcounts) {
      so.push_back(val + 0x30);
    }
    std::sort(so.begin(), so.end());
    std::ranges::reverse(so);
  }

  void populate(std::string str) {
    for (auto ch : str) {
      
      switch (ch) {
        case '2':
          h.push_back(CardValue::TWO);
          continue;
        case '3':
          h.push_back(CardValue::THREE);
          continue;
        case '4':
          h.push_back(CardValue::FOUR);
          continue;
        case '5':
          h.push_back(CardValue::FIVE);
          continue;
        case '6':
          h.push_back(CardValue::SIX);
          continue;
        case '7':
          h.push_back(CardValue::SEVEN);
          continue;
        case '8':
          h.push_back(CardValue::EIGHT);
          continue;
        case '9':
          h.push_back(CardValue::NINE);
          continue;
        case 'T':
          h.push_back(CardValue::TEN);
          continue;
        case 'J':
          h.push_back(CardValue::JACK);
          continue;
        case 'Q':
          h.push_back(CardValue::QUEEN);
        continue;
        case 'K':
          h.push_back(CardValue::KING);
          continue;
        case 'A':
          h.push_back(CardValue::ACE);
          continue;
        default:
          return;
      }
    }
    std::cout << std::endl;
  }
};

using HandVec = std::vector<Hand>;

int main(void) {
  std::cout << (CardValue::FIVE > CardValue::SIX) << std::endl;
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

  std::vector<std::pair<std::string, int64_t>> split_hands {};
  std::stringstream ss;
  for (auto line : lines) {
    ss.clear();
    ss << line;
    int64_t bet_value;
    std::string hand;
    ss >> hand;
    ss >> bet_value;
    split_hands.push_back(std::pair {hand, bet_value});
  }

  std::cout << "Split hands: " << std::endl;
  for (auto& h : split_hands) {
    std::cout << "(" << h.first << ", " << h.second << "), " << std::endl;
  }

  std::vector<std::pair<Hand, int64_t>> real_hands {};
  std::transform(split_hands.begin(), split_hands.end(), std::back_inserter(real_hands), [](auto h){
    Hand localh;
    localh.populate(h.first);
    localh.generate_strongorder();
    return std::pair {localh, h.second};
  });

  std::sort(real_hands.begin(), real_hands.end(), [](auto& a, auto& b){
    return a.first < b.first;
  });

  for (int i = 0; i < real_hands.size(); ++i) {
    real_hands[i].second *= i + 1;
  }

  
  std::vector<int64_t> bets;
  for (auto& h : real_hands) {
    bets.push_back(h.second);
  }


  return std::accumulate(bets.begin(), bets.end(), 0);
}
