#pragma once

#include <poll.h>

#include <functional>
#include <vector>

/**
 * @brief A simple event loop based on poll(2).
 *
 * Allows registering file descriptors with callbacks to be
 * invoked when events occur, including a helper for watching
 * standard input (stdin) for key presses.
 */
class EventLoop {
  using Callback = std::function<void()>;
  using KeyCallback = std::function<void(char)>;

  /**
   * @brief Internal handler structure tying a pollfd and a callback.
   */
  struct Handler {
    pollfd pfd;  ///< pollfd structure holding fd and events
    Callback cb; ///< Callback function to invoke on event
  };

  std::vector<Handler> handlers_; ///< Vector of registered handlers

public:
  /**
   * @brief Default constructor.
   */
  EventLoop() = default;

  /**
   * @brief Default destructor.
   */
  ~EventLoop() = default;

  // Disable copy semantics
  EventLoop(const EventLoop &) = delete;
  EventLoop &operator=(const EventLoop &) = delete;

  // Enable move semantics
  EventLoop(EventLoop &&) = default;
  EventLoop &operator=(EventLoop &&) = default;

  /**
   * @brief Register a file descriptor to be monitored by the event loop.
   *
   * @param fd The file descriptor to watch.
   * @param events Poll events (e.g., POLLIN, POLLOUT) to watch for.
   * @param cb Callback to be called when the fd is ready.
   */
  void addFd(int fd, short events, Callback cb);

  /**
   * @brief Convenience function to watch standard input (fd = 0).
   *
   * Calls the provided callback with each character read from stdin.
   *
   * @param onKey Callback that receives the read character.
   */
  void watchStdin(KeyCallback onKey);

  /**
   * @brief Run the event loop until the quit condition is met.
   *
   * Polls registered file descriptors and calls their callbacks
   * when events occur.
   *
   * @param shouldQuit A function returning true when the loop should exit.
   */
  void run(std::function<bool()> shouldQuit);
};
