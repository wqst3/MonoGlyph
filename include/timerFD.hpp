#pragma once

#include <sys/timerfd.h>
#include <system_error>
#include <cerrno>

#include "fileDescriptor.h"

/**
 * @brief Обработчик таймера с использованием timerfd.
 *
 * @details
 * Этот класс создает файловый дескриптор таймера, который генерирует
 * периодические события с заданной частотой кадров (FPS). Это позволяет
 * интегрировать таймер в цикл событий, используя такие механизмы, как
 * epoll или select.
 */
class TimerFDHandler {
  FileDescriptor fd_; ///< Файловый дескриптор, связанный с timerfd.

 public:
  /**
   * @brief Конструктор, создающий и настраивающий timerfd.
   *
   * @param[in] fps Частота кадров (кадров в секунду), с которой должен срабатывать таймер.
   *
   * @throws std::system_error Если не удалось создать или настроить timerfd.
   */
  explicit TimerFDHandler(unsigned int fps) {
    int fd = timerfd_create(CLOCK_MONOTONIC, TFD_CLOEXEC);
    if (fd == -1) {
      throw std::system_error(errno, std::generic_category(),
                              "timerfd_create failed");
    }

    itimerspec its{};
    its.it_value.tv_nsec = 1'000'000'000 / fps;
    its.it_interval = its.it_value;

    if (timerfd_settime(fd, 0, &its, nullptr) == -1) {
      throw std::system_error(errno, std::generic_category(),
                              "timerfd_settime failed");
    }

    fd_.reset(fd);
  }

  /**
   * @brief Получить файловый дескриптор для чтения событий таймера.
   *
   * @return Ссылка на объект FileDescriptor, связанный с timerfd.
   */
  const FileDescriptor& fd() const noexcept { return fd_; }
};
