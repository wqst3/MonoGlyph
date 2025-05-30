#pragma once

#include <random>

#include "commonTypes.h"
#include "drawer.h"
#include "fontManager.h"
#include "screenBuffer.h"
#include "signalFD.hpp"
#include "state.h"
#include "terminal.h"
#include "timerFD.hpp"

/**
 * @class MonoGlyph
 * @brief Основной класс, управляющий  состояниями и обработкой событий.
 */
class MonoGlyph {
  using Line = Vector<float>;
  using Size = Point<int>;

  bool correctLetterInput_;

  std::unique_ptr<State> currentState_;

  Terminal terminal_;
  ScreenBuffer sBuffer_;
  Drawer drawer_;
  FontManager fManager_;

  std::random_device rd_;
  std::mt19937 gen_;

  SignalFDHandler signalFDHandler_;
  TimerFDHandler timerFDHandler_;

  Glyph leftLetter_;
  Glyph mainLetter_;
  Glyph rightLetter_;

 public:
  /**
   * @brief Конструктор по умолчанию. Инициализирует терминал, загрузку шрифта и
   * состояние.
   */
  MonoGlyph();

  /**
   * @brief Удалённый конструктор копирования.
   */
  MonoGlyph(const MonoGlyph &) = delete;

  /**
   * @brief Удалённый оператор присваивания.
   */
  MonoGlyph &operator=(const MonoGlyph &) = delete;

  /**
   * @brief Деструктор. Восстанавливает состояние терминала.
   */
  ~MonoGlyph();

  /**
   * @brief Проверяет, был ли ввод буквы корректным.
   * @return true, если ввод корректен, иначе false.
   */
  bool correctLetterInput() const noexcept;

  /**
   * @brief Запускает основной цикл приложения.
   * @return 0 при успешном завершении, 1 при системной ошибке, 2 при другой
   * ошибке.
   */
  int start();

  /**
   * @brief Обрабатывает событие изменения размера терминала.
   */
  void onResize();

  /**
   * @brief Инициализирует начальные буквы: центральную и правую, очищает левую.
   */
  void updateLetters();

  /**
   * @brief Обновляет буквы после ввода. Сдвигает текущие и генерирует новую
   * правую.
   * @param correct Флаг корректности ввода пользователем.
   */
  void newLetter(bool correct);

  /**
   * @brief Сменить текущее состояние.
   * @param newState Указатель на новое состояние.
   */
  void changeState(std::unique_ptr<State> newState);

  /**
   * @brief Получить текущее состояние.
   * @return Указатель на текущее состояние.
   */
  State *currentState() noexcept;

  /**
   * @brief Получить объект терминала.
   * @return Ссылка на Terminal.
   */
  Terminal &terminal() noexcept;

  /**
   * @brief Получить экранный буфер.
   * @return Ссылка на ScreenBuffer.
   */
  ScreenBuffer &screenBuffer() noexcept;

  /**
   * @brief Получить объект рисовальщика.
   * @return Ссылка на Drawer.
   */
  Drawer &drawer() noexcept;

  /**
   * @brief Получить менеджер шрифтов.
   * @return Ссылка на FontManager.
   */
  FontManager &fonts() noexcept;

  /**
   * @brief Получить обработчик сигнала SIGWINCH.
   * @return Ссылка на SignalFDHandler.
   */
  SignalFDHandler &signalFDHandler() noexcept;

  /**
   * @brief Получить обработчик таймера.
   * @return Ссылка на TimerFDHandler.
   */
  TimerFDHandler &timerFDHandler() noexcept;

  /**
   * @brief Получить левую букву.
   * @return Ссылка на Glyph.
   */
  Glyph &leftLetter() noexcept;

  /**
   * @brief Получить основную (центральную) букву.
   * @return Ссылка на Glyph.
   */
  Glyph &mainLetter() noexcept;

  /**
   * @brief Получить правую букву.
   * @return Ссылка на Glyph.
   */
  Glyph &rightLetter() noexcept;
};
