#pragma once

#include <vector>

#include "commonTypes.h"
#include "screenBuffer.h"

/**
 * @class Drawer
 * @brief Класс для рисования пикселей, линий, строк и буферов на ScreenBuffer.
 */
class Drawer {
  using Line = Vector<float>;
  using Size = Point<int>;
  ScreenBuffer &sBuffer_;

 public:
  /**
   * @brief Конструктор, инициализирует Drawer с указателем на ScreenBuffer.
   * @param sb Ссылка на ScreenBuffer, на котором будет производиться рисование.
   */
  Drawer(ScreenBuffer &) noexcept;

  /**
   * @brief Рисует пиксель в заданной точке.
   * @param x Координата X.
   * @param y Координата Y.
   * @param c Пиксель (цвет и символ) для рисования.
   */
  void drawPixel(int x, int y, const Pixel &c) noexcept;

  /**
   * @brief Рисует линию между двумя точками алгоритмом Брезенхема.
   * @param x1 Координата X начальной точки.
   * @param y1 Координата Y начальной точки.
   * @param x2 Координата X конечной точки.
   * @param y2 Координата Y конечной точки.
   * @param c Пиксель (цвет и символ) для рисования.
   */
  void drawLine(int x1, int y1, int x2, int y2, const Pixel &c) noexcept;

  /**
   * @brief Рисует динамическую букву в заданной позиции.
   * @param x Координата X.
   * @param y Координата Y.
   * @param c Пиксель для рисования буквы.
   * @return Размер нарисованной буквы (ширина и высота).
   */
  Size drawDynamicLetter(int x, int y, const Pixel &c) noexcept;

  /**
   * @brief Рисует строку из массива пикселей.
   * @param x Координата X начала строки.
   * @param y Координата Y начала строки.
   * @param s Указатель на массив пикселей.
   */
  void drawString(int x, int y, const Pixel *s) noexcept;

  /**
   * @brief Рисует строку из широких символов с заданным цветом текста и фона.
   * @param x Координата X начала строки.
   * @param y Координата Y начала строки.
   * @param s Указатель на широкую строку.
   * @param fg Цвет текста (по умолчанию Color::Default).
   * @param bg Цвет фона (по умолчанию BgColor::Default).
   */
  void drawString(int x, int y, const wchar_t *s, Color fg = Color::Default,
                  BgColor bg = BgColor::Default) noexcept;

  /**
   * @brief Рисует набор линий, нормализованных относительно размера экрана.
   * @param x Смещение по X для рисования.
   * @param y Смещение по Y для рисования.
   * @param lines Вектор линий (каждая линия — Vector<float> с координатами от 0
   * до 1).
   * @param c Пиксель для рисования линий.
   */
  void drawView(int x, int y, const std::vector<Line> &lines,
                const Pixel &c) noexcept;

  /**
   * @brief Рисует содержимое другого ScreenBuffer в текущем ScreenBuffer.
   * @param x Координата X, где будет нарисован буфер.
   * @param y Координата Y, где будет нарисован буфер.
   * @param sb Ссылка на ScreenBuffer для копирования.
   */
  void drawBuffer(int x, int y, ScreenBuffer &sb) noexcept;
};
