#pragma once

#include <unistd.h>

class FileDescriptor {
	int fd_;

public:
	explicit FileDescriptor(int = -1) noexcept;
	~FileDescriptor() noexcept;

	FileDescriptor(FileDescriptor&&) noexcept;
	FileDescriptor& operator=(FileDescriptor&&) noexcept;

	FileDescriptor(const FileDescriptor&) = delete;
	FileDescriptor& operator=(const FileDescriptor&) = delete;

	int get() const noexcept;
	bool valid() const noexcept;
	void reset(int = -1) noexcept;
	void close() noexcept;
};
