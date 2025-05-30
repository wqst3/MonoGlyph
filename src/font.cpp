#include "font.h"

#include <filesystem>
#include <fstream>
#include <sstream>
#include <stdexcept>

// === public methods ===
Font::Font(const std::filesystem::path &path) : name_(path.stem().string()) {
  parseFontFile(path);
}

const std::string Font::getLetters() const noexcept {
  std::string letters;
  letters.reserve(glyphs_.size());
  for (const auto &[key, _] : glyphs_) letters.push_back(key);
  return letters;
}

const Glyph &Font::get(char c) const { return glyphs_.at(c); }

const Glyph *Font::getIfExists(char c) const noexcept {
  auto it = glyphs_.find(c);
  return it != glyphs_.end() ? &it->second : nullptr;
}

// === private methods ===
void Font::parseFontFile(const std::filesystem::path &path) {
  std::ifstream in(path);
  if (!in) throw std::runtime_error("Cannot open font file: " + path.string());

  char current = '\0';
  std::string line;
  while (std::getline(in, line)) {
    if (line.empty()) continue;
    if (line.size() == 1) {
      current = line[0];
      glyphs_[current] = {};
    } else {
      std::istringstream iss(line);
      Line seg;
      if (!(iss >> seg.x1 >> seg.y1 >> seg.x2 >> seg.y2)) continue;
      glyphs_[current].segments.push_back(seg);
    }
  }
}
