#include "terminal.h"

#include <sys/ioctl.h>
#include <unistd.h>

#include <cerrno>
#include <system_error>

// === public methods ===
Terminal::Terminal() { updateSize(); }

Terminal::Size Terminal::updateSize() {
  winsize ws;

  if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1) {
    throw std::system_error(errno, std::generic_category(),
                            "Failed to get terminal size");
  }

  size_.x = ws.ws_col;
  size_.y = ws.ws_row;

  return size_;
}

void Terminal::enableRawMode() {
  if (tcgetattr(STDIN_FILENO, &orig_termios_) == -1) {
    throw std::system_error(errno, std::generic_category(),
                            "Failed to get terminal attributes (tcgetattr)");
  }

  raw_ = orig_termios_;

  raw_.c_lflag &= ~(ECHO | ICANON | ISIG | IEXTEN);
  raw_.c_iflag &= ~(IXON | ICRNL | BRKINT | INPCK | ISTRIP);
  raw_.c_oflag &= ~(OPOST);
  raw_.c_cflag |= (CS8);

  raw_.c_cc[VMIN] = 0;
  raw_.c_cc[VTIME] = 1;

  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw_) == -1) {
    throw std::system_error(errno, std::generic_category(),
                            "Failed to set terminal attributes (tcsetattr)");
  }
}

void Terminal::disableRawMode() {
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios_);
}

void Terminal::altBuffer() {
  safeWrite(STDOUT_FILENO, "\033[?1049h", 8, "altBuffer");
}

void Terminal::hideCur() {
  safeWrite(STDOUT_FILENO, "\x1b[?25l", 6, "hideCur");
}

void Terminal::clear() {
  safeWrite(STDOUT_FILENO, "\x1b[2J", 4, "clear");
  safeWrite(STDOUT_FILENO, "\x1b[H", 3, "clear");
}

void Terminal::restore() {
  safeWrite(STDOUT_FILENO, "\x1b[?25h", 6, "restore");
  safeWrite(STDOUT_FILENO, "\033[?1049l", 8, "restore");
}

// === private methods ===
void Terminal::safeWrite(int fd, const char *data, size_t len,
                         const char *desc) {
  if (write(fd, data, len) != static_cast<ssize_t>(len)) {
    throw std::runtime_error(std::string("Terminal write failed: ") + desc);
  }
}
