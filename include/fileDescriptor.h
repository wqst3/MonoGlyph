#pragma once

#include <unistd.h>

/**
 * @class FileDescriptor
 * @brief RAII wrapper for managing a Unix file descriptor.
 *
 * This class automatically closes the file descriptor when the object goes out
 * of scope, ensuring proper resource management and preventing descriptor
 * leaks.
 */
class FileDescriptor {
  int fd_; ///< The managed file descriptor.

public:
  /**
   * @brief Constructs a FileDescriptor managing the given descriptor.
   * @param fd File descriptor to manage. Default is -1 (invalid descriptor).
   */
  explicit FileDescriptor(int fd = -1) noexcept;

  /**
   * @brief Destructor that automatically closes the managed file descriptor if
   * valid.
   */
  ~FileDescriptor() noexcept;

  /**
   * @brief Move constructor.
   * Transfers ownership of the file descriptor from another FileDescriptor
   * object. After the move, the other object's descriptor is set to -1.
   * @param other The other FileDescriptor to move from.
   */
  FileDescriptor(FileDescriptor &&other) noexcept;

  /**
   * @brief Move assignment operator.
   * Closes the current descriptor (if valid), then transfers ownership
   * from the other object. The other object's descriptor is set to -1.
   * @param other The other FileDescriptor to move from.
   * @return Reference to this object.
   */
  FileDescriptor &operator=(FileDescriptor &&other) noexcept;

  /// Copy constructor is deleted to avoid copying of file descriptor ownership.
  FileDescriptor(const FileDescriptor &) = delete;

  /// Copy assignment operator is deleted to avoid copying of file descriptor
  /// ownership.
  FileDescriptor &operator=(const FileDescriptor &) = delete;

  /**
   * @brief Returns the current managed file descriptor.
   * @return The file descriptor as an integer.
   */
  int get() const noexcept;

  /**
   * @brief Checks if the file descriptor is valid.
   * @return true if the descriptor is valid (not -1), false otherwise.
   */
  bool valid() const noexcept;

  /**
   * @brief Closes the currently managed descriptor (if valid) and takes
   * ownership of a new one.
   * @param newFd New file descriptor to manage. Default is -1 (invalid).
   */
  void reset(int newFd = -1) noexcept;

  /**
   * @brief Closes the managed file descriptor if it is valid.
   * After this call, the descriptor is set to -1.
   */
  void close() noexcept;
};
