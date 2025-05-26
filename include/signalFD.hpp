#pragma once

#include "fileDescriptor.h"

#include <signal.h>
#include <sys/signalfd.h>

class SignalFDHandler
{
	sigset_t mask_;
	FileDescriptor fd_;

public:
	explicit SignalFDHandler(int signal)
	{	
		sigemptyset(&mask_);
		sigaddset(&mask_, signal);

		if (sigprocmask(SIG_BLOCK, &mask_, nullptr) == -1) {
			throw std::system_error(errno, std::generic_category(), "sigprocmask failed");
		}

		int fd = signalfd(-1, &mask_, SFD_CLOEXEC);
		if (fd == -1) {
			throw std::system_error(errno, std::generic_category(), "signalfd failed");
		}

		fd_.reset(fd);
	}

	const FileDescriptor& fd() const noexcept { return fd_; }

};

