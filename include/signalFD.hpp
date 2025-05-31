#pragma once

#include <signal.h>
#include <sys/signalfd.h>
#include <system_error>
#include <cerrno>

#include "fileDescriptor.h"

/**
 * @class SignalFDHandler
 * @brief A RAII-style wrapper around `signalfd` for handling signals using file descriptors.
 *
 * This class sets up a file descriptor that can be used to monitor a specific signal.
 * It blocks the given signal and then creates a signalfd which can be polled or selected on.
 */
class SignalFDHandler {
  sigset_t mask_;     ///< Signal mask used to block and watch the specified signal.
  FileDescriptor fd_; ///< File descriptor for the signal.

 public:
  /**
   * @brief Constructs a SignalFDHandler for a given signal.
   *
   * Blocks the specified signal and creates a `signalfd` to receive it.
   *
   * @param signal The signal number to watch (e.g., SIGTERM, SIGINT).
   *
   * @throws std::system_error If `sigprocmask` or `signalfd` fails.
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
   * @brief Returns the internal file descriptor.
   *
   * @return A constant reference to the FileDescriptor object.
   */
  const FileDescriptor& fd() const noexcept { return fd_; }
};
