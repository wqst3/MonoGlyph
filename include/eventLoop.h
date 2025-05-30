#pragma once

#include <poll.h>

#include <functional>
#include <vector>

class EventLoop {
  using Callback = std::function<void()>;
  using KeyCallback = std::function<void(char)>;

  struct Handler {
    pollfd pfd;
    Callback cb;
  };
  std::vector<Handler> handlers_;

 public:
  EventLoop() = default;
  ~EventLoop() = default;

  EventLoop(const EventLoop &) = delete;
  EventLoop &operator=(const EventLoop &) = delete;
  EventLoop(EventLoop &&) = default;
  EventLoop &operator=(EventLoop &&) = default;
  /**
   * @brief Adds a file descriptor to be monitored by the event loop.
   *
   * This function registers a file descriptor and the set of events (e.g.,
   * POLLIN, POLLOUT) to watch. When one of the specified events occurs, the
   * corresponding callback will be invoked.
   *
   * @param fd The file descriptor to monitor.
   * @param events A bitmask of events (e.g., POLLIN) that should be monitored
   * for the fd.
   * @param cb The callback function to be invoked when the specified events
   * occur.
   */
  void addFd(int fd, short events, Callback cb);

  /**
   * @brief Watches standard input (stdin) for key presses.
   *
   * This is a convenience function that monitors stdin (file descriptor 0) for
   * input. When a character is read, the provided callback is called with the
   * character as an argument.
   *
   * @param onKey The callback function to be called when a key is pressed. The
   * function receives the pressed character as its argument.
   */
  void watchStdin(KeyCallback onKey);

  /**
   * @brief Runs the event loop until a stop condition is met.
   *
   * This function enters a loop where it waits for events on the registered
   * file descriptors. When an event is detected, the associated callback is
   * executed. The loop continues to run until the provided function returns
   * true.
   *
   * @param shouldQuit A function that returns true when the event loop should
   * stop running.
   */
  void run(std::function<bool()> shouldQuit);
};
