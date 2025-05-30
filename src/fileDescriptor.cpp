#include "fileDescriptor.h"

#include <utility>

// === public methods ===
FileDescriptor::FileDescriptor(int fd) noexcept : fd_(fd) {}

FileDescriptor::~FileDescriptor() noexcept { close(); }

FileDescriptor::FileDescriptor(FileDescriptor &&other) noexcept
    : fd_(other.fd_) {
  other.fd_ = -1;
}

FileDescriptor &FileDescriptor::operator=(FileDescriptor &&other) noexcept {
  if (this != &other) {
    close();
    fd_ = other.fd_;
    other.fd_ = -1;
  }
  return *this;
}

int FileDescriptor::get() const noexcept { return fd_; }

bool FileDescriptor::valid() const noexcept { return fd_ != -1; }

void FileDescriptor::reset(int newFd) noexcept {
  if (fd_ != newFd) {
    close();
    fd_ = newFd;
  }
}

void FileDescriptor::close() noexcept {
  if (fd_ != -1) {
    ::close(fd_);
    fd_ = -1;
  }
}
