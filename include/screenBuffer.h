#pragma once

#include "commonTypes.h"
#include "pixel.h"

/**
 * @class ScreenBuffer
 * @brief Управляет двойным буфером для отрисовки пикселей на экране.
 */
class ScreenBuffer {
  using Size = Point<int>;

  Pixel *buffer_;     /**< Текущий буфер пикселей. */
  Pixel *prevBuffer_; /**< Предыдущий буфер для сравнения изменений. */
  Size size_;         /**< Размер буфера (ширина x высота). */

  /**
   * @brief Выделяет память под буферы заданного размера.
   * @param cols Количество столбцов (ширина).
   * @param rows Количество строк (высота).
   */
  void allocate(int cols, int rows) noexcept;

  /**
   * @brief Освобождает выделенную память буферов.
   */
  void deallocate() noexcept;

 public:
  /**
   * @brief Конструктор по умолчанию, создаёт пустой буфер.
   */
  ScreenBuffer() noexcept;

  /**
   * @brief Конструктор с заданными размерами буфера.
   * @param cols Количество столбцов.
   * @param rows Количество строк.
   */
  ScreenBuffer(int cols, int rows) noexcept;

  /**
   * @brief Конструктор с заданным размером.
   * @param size Размер буфера (ширина и высота).
   */
  ScreenBuffer(Size size) noexcept;

  ScreenBuffer(const ScreenBuffer &) = delete; /**< Запрет копирования. */
  ScreenBuffer &operator=(const ScreenBuffer &) =
      delete; /**< Запрет присваивания. */

  /**
   * @brief Деструктор, освобождает память буферов.
   */
  ~ScreenBuffer() noexcept;

  /**
   * @brief Копирует текущий буфер в предыдущий для синхронизации.
   */
  void syncBuffers() const noexcept;

  /**
   * @brief Изменяет размер буфера, освобождая старую память и выделяя новую.
   * Очищает буфер после изменения размера.
   * @param cols Новое количество столбцов.
   * @param rows Новое количество строк.
   */
  void resize(int cols, int rows) noexcept;

  /**
   * @brief Заполняет текущий буфер указанным пикселем.
   * @param fillChar Пиксель для заполнения (по умолчанию пробел).
   */
  void clear(Pixel fillChar = L' ') noexcept;

  /**
   * @brief Выводит на экран только изменённые пиксели с учётом цвета.
   */
  void flush() const noexcept;

  /**
   * @brief Получить пиксель по координатам (константный доступ).
   * @param x Координата X (столбец).
   * @param y Координата Y (строка).
   * @return Константная ссылка на пиксель.
   */
  const Pixel &at(int x, int y) const noexcept;

  /**
   * @brief Получить пиксель по координатам (изменяемый доступ).
   * @param x Координата X (столбец).
   * @param y Координата Y (строка).
   * @return Ссылка на пиксель.
   */
  Pixel &at(int x, int y) noexcept;

  /**
   * @brief Возвращает текущий размер буфера.
   * @return Объект Size с шириной и высотой.
   */
  Size size() const noexcept { return size_; }
};
