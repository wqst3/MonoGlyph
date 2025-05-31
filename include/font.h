#pragma once

#include <filesystem>
#include <string>
#include <unordered_map>
#include <vector>

#include "commonTypes.h"

/**
 * @brief Represents a glyph in the font, composed of line segments.
 */
struct Glyph {
  /// Vector of line segments that make up the glyph
  std::vector<Vector<float>> segments;
};

/**
 * @brief Class representing a vector font loaded from a file.
 *
 * The font consists of glyphs, each represented by a set of line segments.
 * The font file format:
 * - Each line containing a single character indicates a new glyph.
 * - Each line containing four float numbers specifies a line segment for the
 * current glyph.
 */
class Font {
  using Line = Vector<float>; ///< Alias for a line segment type
  using Size = Point<int>;    ///< Alias for size type (currently unused)

  std::string name_; ///< Name of the font (filename without extension)
  std::unordered_map<char, Glyph>
      glyphs_; ///< Map from characters to their glyphs

  /**
   * @brief Parses a font file and populates the glyphs_ map.
   *
   * The parsing rules:
   * - A line with a single character starts a new glyph definition.
   * - Lines with four float numbers specify line segments for the current
   * glyph.
   *
   * @param path Filesystem path to the font file.
   * @throws std::runtime_error if the file cannot be opened.
   */
  void parseFontFile(const std::filesystem::path &path);

public:
  /**
   * @brief Constructs a Font object by loading it from a file.
   *
   * The font name is extracted from the filename without its extension.
   *
   * @param path Filesystem path to the font file.
   * @throws std::runtime_error if the file cannot be opened.
   */
  explicit Font(const std::filesystem::path &path);

  /**
   * @brief Returns a string containing all characters available in the font.
   *
   * @return String of all characters (keys) for which glyphs exist.
   */
  const std::string getLetters() const noexcept;

  /**
   * @brief Returns a constant reference to the Glyph for a given character.
   *
   * @param c Character whose glyph is requested.
   * @return Constant reference to the Glyph.
   * @throws std::out_of_range if the character is not found in the font.
   */
  const Glyph &get(char c) const;

  /**
   * @brief Returns a pointer to the Glyph for a given character if it exists.
   *
   * @param c Character to search for.
   * @return Pointer to the Glyph if found, nullptr otherwise.
   */
  const Glyph *getIfExists(char c) const noexcept;

  /**
   * @brief Returns the name of the font.
   *
   * @return Font name as a std::string.
   */
  const std::string name() const noexcept { return name_; }
};
