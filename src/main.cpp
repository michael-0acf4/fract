#include <cmath>
#include <iostream>
#include <map>

typedef std::map<char, std::pair<float, float>> range_table;

range_table build_prob(std::string &text) {
  std::map<char, float> prob;
  // count
  for (const char &c : text)
    prob[c] += 1.;
  // ranges
  float total = float(text.size());
  float lo = 0.;
  range_table result;
  for (const auto &[c, v] : prob) {
    float hi = lo + v / total;
    result[c] = {lo, hi};
    lo = hi;
  }
  return result;
}

float encode(std::string &text, range_table &prob) {
  float l = 0., r = 1.;
  for (const char &c : text) {
    float width = r - l;
    auto [focus_lo, focus_hi] = prob[c];
    float new_l = l + width * focus_lo;
    r = l + width * focus_hi;
    l = new_l;
  }
  return (l + r) / 2;
}
std::string decode(size_t msg_len, float encoded, range_table &prob) {
  float l = 0., r = 1.;
  std::string decoded = "";
  for (size_t i = 0; i < msg_len; i++) {
    float width = r - l;
    float scaled = (encoded - l) / width;
    for (const auto &[c, v] : prob) {
      auto [focus_lo, focus_hi] = v;
      if (focus_lo <= scaled && scaled < focus_hi) {
        decoded += c;
        float new_l = l + width * focus_lo;
        r = l + width * focus_hi;
        l = new_l;
        break;
      }
    }
  }
  return decoded;
}

std::string code(float x) {
  std::string out = "";
  float base = 2;
  while (true) {
    float n = x * base;
    float c = n - floor(n);
    out += int(n) + '0';
    if (c < 10e-6)
      break;
    x = c;
  }
  return out;
}

int main() {
  std::string text = "this is a";
  auto prob = build_prob(text);
  for (auto c : prob) {
    std::cout << c.first << ":" << c.second.first << ", " << c.second.second
              << std::endl;
  }

  float encoded = encode(text, prob);
  std::string decoded = decode(text.size(), encoded, prob);
  std::cout << "\n" << encoded;
  std::cout << "\n" << decoded;
  // std::cout << code(0.5) << " vs " << code(0.8) << std::endl;
  return 0;
}
