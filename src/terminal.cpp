#include "terminal.h"

#include <system_error>
#include <cerrno>
#include <unistd.h>
#include <sys/ioctl.h>

// === public methods ===
Terminal::Terminal()
{
	updateSize();
}


void Terminal::updateSize()
{	
	winsize ws;

	if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1)
	{
		throw std::system_error(errno, std::generic_category(), "Failed to get terminal size");
	}

	size_.x = ws.ws_col;
	size_.y = ws.ws_row;
}


void Terminal::enableRawMode()
{
	if (tcgetattr(STDIN_FILENO, &orig_termios_) == -1)
	{
		throw std::system_error(errno, std::generic_category(), "Failed to get terminal attributes (tcgetattr)");
	}

	raw_ = orig_termios_;

	raw_.c_lflag &= ~(ECHO | ICANON | ISIG | IEXTEN);
	raw_.c_iflag &= ~(IXON | ICRNL | BRKINT | INPCK | ISTRIP);
	raw_.c_oflag &= ~(OPOST);
	raw_.c_cflag |= (CS8);

	raw_.c_cc[VMIN] = 0;
	raw_.c_cc[VTIME] = 1;

	if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw_) == -1)
	{
		throw std::system_error(errno, std::generic_category(), "Failed to set terminal attributes (tcsetattr)");
	}
}

void Terminal::disableRawMode()
{
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios_);
}


void Terminal::clear()
{
	std::system("tput smcup");		// Включить альтернативный экранный буфер
	write(STDOUT_FILENO, "\x1b[?25l", 6);   // Скрыть курсор
	write(STDOUT_FILENO, "\x1b[2J", 4);     // Очистить весь экран
	write(STDOUT_FILENO, "\x1b[H", 3);      // Курсор в начало
}

void Terminal::restore()
{	
	write(STDOUT_FILENO, "\x1b[?25h", 6);   // Показать курсор
	std::system("tput rmcup");		// Выйти из альтернативного буфера
}

