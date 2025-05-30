#pragma once

#include "MonoGlyph.h"

/**
 * @class InterfaceDrawer
 * @brief Класс для отрисовки интерфейса в приложении MonoGlyph.
 *
 * Использует объект MonoGlyph для доступа к терминалу и отрисовке различных
 * элементов интерфейса, таких как меню, кнопки и буквы.
 */
class InterfaceDrawer {
  using Line = Vector<float>;
  using Size = Point<int>;

  MonoGlyph &MonoGlyph_;

 public:
  /**
   * @brief Конструктор класса InterfaceDrawer.
   * @param MonoGlyph Ссылка на объект MonoGlyph, используемый для отрисовки.
   */
  InterfaceDrawer(MonoGlyph &MonoGlyph);

  /**
   * @brief Отрисовывает полный интерфейс, включая меню, основную и боковые
   * буквы, а также кнопку перезапуска.
   */
  void menu();

  /**
   * @brief Отрисовывает верхнюю часть меню с заголовком и выбором режима.
   */
  void upMenu();

  /**
   * @brief Отрисовывает кнопку перезапуска в нижней части интерфейса.
   */
  void restartButton();

  /**
   * @brief Отрисовывает основную букву в центре экрана.
   */
  void mainLetter();

  /**
   * @brief Отрисовывает левую боковую букву, отображающую правильность ввода.
   */
  void leftLetter();

  /**
   * @brief Отрисовывает правую боковую букву.
   */
  void rightLetter();
};
