#pragma once

#include "fileDescriptor.h"

#include <sys/timerfd.h>

class TimerFDHandler
{
	FileDescriptor fd_;

public:
	explicit TimerFDHandler(unsigned int fps)
	{
		int fd = timerfd_create(CLOCK_MONOTONIC, TFD_CLOEXEC);
		if (fd == -1) {
			throw std::system_error(errno, std::generic_category(), "timerfd_create failed");
		}

		itimerspec its{};
		its.it_value.tv_nsec = 1'000'000'000 / fps;
		its.it_interval = its.it_value;

		if (timerfd_settime(fd, 0, &its, nullptr) == -1) {
			throw std::system_error(errno, std::generic_category(), "timerfd_settime failed");
		}

		fd_.reset(fd);
	}

	const FileDescriptor& fd() const noexcept { return fd_; }

};

