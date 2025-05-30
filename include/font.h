#pragma once

#include <filesystem>
#include <string>
#include <unordered_map>
#include <vector>

#include "commonTypes.h"

struct Glyph {
  std::vector<Vector<float>> segments;
};

class Font {
  using Line = Vector<float>;
  using Size = Point<int>;

  std::string name_;
  std::unordered_map<char, Glyph> glyphs_;

  /**
   * @brief Разбирает файл шрифта, заполняя glyphs.
   *
   * Каждая строка с одним символом обозначает новый glyph.
   * Строки с четырьмя числами задают линии glyph.
   *
   * @param path Путь к файлу шрифта.
   * @throws std::runtime_error Если файл не может быть открыт.
   * */
  void parseFontFile(const std::filesystem::path &path);

 public:
  /**
   * @brief Загружает шрифт из файла и инициализирует объект Font.
   *
   * Имя шрифта устанавливается из имени файла без расширения.
   *
   * @param path Путь к файлу шрифта.
   * @throws std::runtime_error Если файл нельзя открыть.
   */
  explicit Font(const std::filesystem::path &path);

  /**
   * @brief Возвращает строку, содержащую все символы, представленные в шрифте.
   *
   * @return Строка со всеми символами (glyphs) шрифта.
   */
  const std::string getLetters() const noexcept;

  /**
   * @brief Возвращает ссылку на glyph, соответствующий символу.
   *
   * @param c Символ для получения glyph.
   * @return Константная ссылка на Glyph.
   * @throws std::out_of_range Если символ не найден.
   */
  const Glyph &get(char c) const;

  /**
   * @brief Возвращает указатель на glyph, если он существует.
   *
   * @param c Символ для поиска.
   * @return Указатель на Glyph, либо nullptr если символ не найден.
   */
  const Glyph *getIfExists(char c) const noexcept;

  /**
   * @brief Возвращает имя шрифта.
   *
   * @return Имя шрифта.
   */
  const std::string name() const noexcept { return name_; }
};
