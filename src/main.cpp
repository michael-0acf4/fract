#include <cmath>
#include <iostream>
#include <map>
#include <vector>

#define CONTAINER float
#define STRINGIFY(x) #x // arg to string
#define TOSTRING(x) STRINGIFY(x)

typedef CONTAINER number;
typedef std::pair<number, number> interval;
typedef std::map<std::string, interval> range_table;

struct EncodedResult {
  number enc;
  range_table prob;

  size_t mem_alloc() const {
    size_t total = sizeof(enc);
    for (const auto &[k, v] : this->prob) {
      total += k.size() + sizeof(v);
    }
    return total;
  }
};

inline std::string get_block(size_t len, size_t block, size_t pos,
                             std::string text) {
  return text.substr(pos, std::min(len, static_cast<size_t>(block)));
}

range_table build_prob(size_t block, const std::string &text) {
  std::map<std::string, number> prob;
  size_t length = text.size();

  number total = 0;
  for (std::size_t i = 0; i < length; i += block) {
    std::string in = get_block(text.size(), block, i, text);
    prob[in] += 1.;
    total += 1.;
  }

  number lo = 0.;
  range_table result;
  for (const auto &[b, v] : prob) {
    number hi = lo + v / total;
    result[b] = {lo, hi};
    lo = hi;
  }

  return result;
}

EncodedResult encode(uint8_t block, const std::string &text) {
  range_table prob = build_prob(block, text);

  number l = 0., r = 1.;
  std::size_t length = text.size();
  for (std::size_t i = 0; i < length; i += block) {
    std::string b = get_block(length, block, i, text);
    number width = r - l;
    auto [focus_lo, focus_hi] = prob[b];
    number new_l = l + width * focus_lo;
    r = l + width * focus_hi;
    l = new_l;
  }

  return {(l + r) / 2, prob};
}

std::string decode(uint8_t block, size_t msg_len,
                   const EncodedResult &encoded) {
  number l = 0., r = 1.;
  std::string decoded = "";

  for (size_t i = 0; i < msg_len; i += block) {
    number width = r - l;
    number scaled = (encoded.enc - l) / width;

    for (const auto &[b, v] : encoded.prob) {
      const auto &[focus_lo, focus_hi] = v;
      if (focus_lo <= scaled && scaled < focus_hi) {
        decoded.append(b);
        number new_l = l + width * focus_lo;
        r = l + width * focus_hi;
        l = new_l;
        break;
      }
    }
  }

  return decoded;
}

int main() {
  // clang-format off
  std::vector<std::string> samples{
    "Hello World",
    "000000000000000000000000000000000000000000000",
    "1234567891234567890",
    "00000000000000000000011111111111111111111111",
    "001111110000000000111111111111111111111111111111111111111111111111111111111111111"
  };
  // clang-format on

  uint8_t block = 4;
  for (const auto &text : samples) {
    auto enc = encode(block, text);
    auto out = decode(block, text.size(), enc);
    std::cout << "Decoded: " << out << '\n';

    std::cout << "Statistics:" << '\n';
    size_t original = sizeof(char) * text.size();
    size_t encoded = enc.mem_alloc();
    float efficiency =
        100 * (float(encoded) - float(original)) / float(original);

    std::cout << " Original: " << original << " bytes" << '\n';
    std::cout << " Stored as: " << enc.enc << " (" << sizeof(enc.enc)
              << " bytes)" << '\n';
    std::cout << " Encoded: " << encoded << " bytes" << '\n';
    std::cout << " Eff: " << (efficiency < 0 ? "-" : "+") << fabs(efficiency)
              << " %" << '\n';

    std::cout << " Correct: ";
    if (out.compare(text) != 0) {
      std::cout << "Badly decoded (try (>sizeof(" << STRINGIFY(CONTAINER)
                << ")) and/or use a smaller block)";
    } else {
      std::cout << "OK";
    }

    std::cout << "\n\n";
  }

  return 0;
}
