#include "eventLoop.h"

#include <unistd.h>

#include <iostream>

void EventLoop::addFd(int fd, short events, Callback cb) {
  pollfd pfd{fd, events, 0};
  handlers_.push_back({pfd, std::move(cb)});
}

void EventLoop::watchStdin(KeyCallback onKey) {
  addFd(0, POLLIN, [onKey = std::move(onKey)]() {
    char ch;
    if (read(0, &ch, 1) == 1) {
      onKey(ch);
    }
  });
}

void EventLoop::run(std::function<bool()> shouldQuit) {
  while (!shouldQuit()) {
    std::vector<pollfd> pfds;
    pfds.reserve(handlers_.size());
    for (auto &h : handlers_) pfds.push_back(h.pfd);

    int ret = ::poll(pfds.data(), pfds.size(), -1);
    if (ret == -1) {
      if (errno == EINTR) continue;
      throw std::system_error(errno, std::generic_category(), "poll failed");
    }

    for (size_t i = 0; i < pfds.size(); ++i) {
      if (pfds[i].revents != 0) {
        handlers_[i].pfd.revents = 0;
        handlers_[i].cb();
      }
    }
  }
}
