#pragma once

#include <unistd.h>

/**
 * @class FileDescriptor
 * @brief RAII-обёртка для управления файловым дескриптором Unix.
 */
class FileDescriptor {
  int fd_;

 public:
  /**
   * @brief Конструктор с параметром по умолчанию.
   * @param fd Файловый дескриптор для управления. По умолчанию -1
   * (недействителен).
   */
  explicit FileDescriptor(int fd = -1) noexcept;

  /**
   * @brief Деструктор, автоматически закрывающий файловый дескриптор.
   */
  ~FileDescriptor() noexcept;

  /**
   * @brief Перемещающий конструктор.
   * @param other Другой объект FileDescriptor, из которого перемещается
   * дескриптор.
   */
  FileDescriptor(FileDescriptor&& other) noexcept;

  /**
   * @brief Перемещающий оператор присваивания.
   * @param other Другой объект FileDescriptor, из которого перемещается
   * дескриптор.
   * @return Ссылка на текущий объект после присваивания.
   */
  FileDescriptor& operator=(FileDescriptor&& other) noexcept;

  // Копирование запрещено
  FileDescriptor(const FileDescriptor&) = delete;
  FileDescriptor& operator=(const FileDescriptor&) = delete;

  /**
   * @brief Получить текущий файловый дескриптор.
   * @return Целочисленный файловый дескриптор.
   */
  int get() const noexcept;

  /**
   * @brief Проверка действительности файлового дескриптора.
   * @return true, если дескриптор действителен; false в противном случае.
   */
  bool valid() const noexcept;

  /**
   * @brief Закрыть текущий дескриптор (если он открыт) и установить новый.
   * @param newFd Новый файловый дескриптор. По умолчанию -1.
   */
  void reset(int newFd = -1) noexcept;

  /**
   * @brief Закрывает файловый дескриптор, если он действителен.
   */
  void close() noexcept;
};
