#pragma once

#include <functional>
#include <vector>
#include <poll.h>

class EventLoop {
	using Callback = std::function<void()>;
	using KeyCallback = std::function<void(char)>;

	struct Handler { pollfd pfd; Callback cb; };
	std::vector<Handler> handlers_;

public:
	EventLoop() = default;
	~EventLoop() = default;

	EventLoop(const EventLoop&) = delete;
	EventLoop& operator=(const EventLoop&) = delete;
	EventLoop(EventLoop&&) = default;
	EventLoop& operator=(EventLoop&&) = default;

	void addFd(int, short, Callback);
	void watchStdin(KeyCallback);
	void run(std::function<bool()>);

};

