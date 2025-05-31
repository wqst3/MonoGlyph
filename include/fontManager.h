#pragma once

#include <filesystem>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "commonTypes.h"
#include "font.h"

/**
 * @brief Manages loading and accessing font resources.
 *
 * This class handles scanning a directory for font files,
 * loading them into memory, and providing access to loaded fonts.
 */
class FontManager {
  std::filesystem::path fontsDir_; ///< Directory where font files are located
  std::unordered_map<std::string, std::shared_ptr<Font>>
      fonts_; ///< Map from font names to Font instances

  /**
   * @brief Find all font files in the fonts directory.
   *
   * Scans the configured fonts directory for files with the ".font" extension.
   *
   * @return A vector of paths to font files found.
   * @throws std::system_error if the fonts directory does not exist.
   */
  std::vector<std::filesystem::path> findFontFiles() const;

public:
  /**
   * @brief Constructs a FontManager.
   *
   * @param fontsDir Path to the directory containing font files.
   *                 Defaults to "./fonts".
   */
  explicit FontManager(const std::filesystem::path &fontsDir = "./fonts");

  /**
   * @brief Loads all font files from the fonts directory.
   *
   * Clears any previously loaded fonts and reloads fonts from disk.
   *
   * @throws std::system_error if fonts directory is missing or
   *         if any font file fails to load.
   */
  void load();

  /**
   * @brief Lists the names of all loaded fonts.
   *
   * @return A vector of font names currently loaded.
   */
  std::vector<std::string> listFontNames() const;

  /**
   * @brief Gets a loaded font by name.
   *
   * @param fontName The name of the font to retrieve.
   * @return A shared pointer to the Font if found, otherwise nullptr.
   */
  std::shared_ptr<Font> get(const std::string &fontName) const;

  /**
   * @brief Checks if a font is loaded.
   *
   * @param fontName The name of the font to check.
   * @return true if the font is loaded, false otherwise.
   */
  bool has(const std::string &fontName) const noexcept;
};
