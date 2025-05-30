#pragma once

#include <filesystem>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "commonTypes.h"
#include "font.h"

/**
 * @class FontManager
 * @brief Управляет загрузкой и хранением шрифтов из указанной директории.
 */
class FontManager {
  std::filesystem::path fontsDir_;  ///< Директория с файлами шрифтов
  std::unordered_map<std::string, std::shared_ptr<Font>>
      fonts_;  ///< Загруженные шрифты (имя -> объект)

  /**
   * @brief Поиск файлов шрифтов в директории.
   * @return Вектор путей к найденным файлам с расширением ".font".
   * @throws std::system_error если директория не существует.
   */
  std::vector<std::filesystem::path> findFontFiles() const;

 public:
  /**
   * @brief Конструктор.
   * @param fontsDir Путь к директории со шрифтами (по умолчанию "./fonts").
   */
  explicit FontManager(const std::filesystem::path &fontsDir = "./fonts");

  /**
   * @brief Загружает все шрифты из директории.
   * Очищает текущие шрифты и загружает заново.
   * @throws std::system_error при ошибках загрузки.
   */
  void load();

  /**
   * @brief Получить список имён загруженных шрифтов.
   * @return Вектор строк с именами шрифтов.
   */
  std::vector<std::string> listFontNames() const;

  /**
   * @brief Получить указатель на шрифт по имени.
   * @param fontName Имя шрифта.
   * @return shared_ptr на Font, или nullptr если не найден.
   */
  std::shared_ptr<Font> get(const std::string &fontName) const;

  /**
   * @brief Проверить, загружен ли шрифт с данным именем.
   * @param fontName Имя шрифта.
   * @return true если шрифт найден, иначе false.
   */
  bool has(const std::string &fontName) const noexcept;
};
