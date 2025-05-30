#pragma once

#include <signal.h>
#include <sys/signalfd.h>
#include <system_error>
#include <cerrno>

#include "fileDescriptor.h"

/**
 * @brief Обработчик сигналов с использованием signalfd.
 *
 * @details
 * Этот класс блокирует указанный сигнал и создает файловый дескриптор,
 * через который можно получать уведомления о поступлении этого сигнала.
 * Это позволяет обрабатывать сигналы в неблокирующем режиме,
 * интегрируя их в цикл событий.
 */
class SignalFDHandler {
  sigset_t mask_; ///< Маска сигналов, содержащая только один указанный сигнал.
  FileDescriptor fd_; ///< Файловый дескриптор, связанный с signalfd.

 public:
  /**
   * @brief Конструктор, блокирующий сигнал и создающий signalfd.
   *
   * @param[in] signal Номер сигнала, который необходимо обрабатывать.
   *
   * @throws std::system_error Если не удалось заблокировать сигнал или создать signalfd.
   */
  explicit SignalFDHandler(int signal) {
    sigemptyset(&mask_);
    sigaddset(&mask_, signal);

    if (sigprocmask(SIG_BLOCK, &mask_, nullptr) == -1) {
      throw std::system_error(errno, std::generic_category(),
                              "sigprocmask failed");
    }

    int fd = signalfd(-1, &mask_, SFD_CLOEXEC);
    if (fd == -1) {
      throw std::system_error(errno, std::generic_category(),
                              "signalfd failed");
    }

    fd_.reset(fd);
  }

  /**
   * @brief Получить файловый дескриптор для чтения сигналов.
   *
   * @return Ссылка на объект FileDescriptor, связанный с signalfd.
   */
  const FileDescriptor& fd() const noexcept { return fd_; }
};
