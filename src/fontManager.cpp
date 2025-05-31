#include "fontManager.h"

#include <cerrno>
#include <iostream>
#include <system_error>

// === public methods ===
FontManager::FontManager(const std::filesystem::path &fontsDir)
    : fontsDir_(fontsDir) {}

void FontManager::load() {
  fonts_.clear();
  auto files = findFontFiles();

  for (const auto &file : files) {
    try {
      auto font = std::make_shared<Font>(file);
      fonts_[font->name()] = font;
    } catch (const std::exception &e) {
      throw std::system_error(
          errno, std::generic_category(),
          "Failed to load font from file: " + file.string() + ". " + e.what());
    }
  }
}

std::vector<std::string> FontManager::listFontNames() const {
  std::vector<std::string> names;
  for (const auto &[name, _] : fonts_) {
    names.push_back(name);
  }
  return names;
}

std::shared_ptr<Font> FontManager::get(const std::string &fontName) const {
  auto it = fonts_.find(fontName);
  if (it != fonts_.end()) {
    return it->second;
  }
  return nullptr;
}

bool FontManager::has(const std::string &fontName) const noexcept {
  return fonts_.find(fontName) != fonts_.end();
}

// === private methods ===
std::vector<std::filesystem::path> FontManager::findFontFiles() const {
  std::vector<std::filesystem::path> result;

  if (!std::filesystem::exists(fontsDir_)) {
    throw std::system_error(errno, std::generic_category(),
                            "Fonts directory does not exist: " +
                                fontsDir_.string());
  }

  for (const auto &entry : std::filesystem::directory_iterator(fontsDir_)) {
    if (entry.is_regular_file() && entry.path().extension() == ".font") {
      result.push_back(entry.path());
    }
  }

  return result;
}
