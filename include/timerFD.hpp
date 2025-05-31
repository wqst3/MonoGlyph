#pragma once

#include <sys/timerfd.h>
#include <system_error>
#include <cerrno>

#include "fileDescriptor.h"

/**
 * @class TimerFDHandler
 * @brief A RAII wrapper for Linux timer file descriptors to generate periodic events.
 *
 * This class sets up a timer using `timerfd_create` that fires at a rate specified
 * by frames per second (FPS). It manages the lifetime of the file descriptor and
 * provides access to it.
 */
class TimerFDHandler {
  FileDescriptor fd_;  ///< File descriptor associated with the timer.

 public:
  /**
   * @brief Constructs a TimerFDHandler with a specified frames per second rate.
   *
   * This sets up a timer that triggers periodically according to the provided FPS.
   *
   * @param fps Frames per second, used to compute timer interval.
   * @throws std::system_error if timer creation or configuration fails.
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
   * @brief Gets the file descriptor associated with the timer.
   *
   * @return A const reference to the FileDescriptor instance.
   */
  const FileDescriptor& fd() const noexcept { return fd_; }
};
