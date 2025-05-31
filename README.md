<div align="center"><pre> __  __                    ____ _             _
   |  \/  | ___  _ __   ___  / ___| |_   _ _ __ | |__
    | |\/| |/ _ \| '_ \ / _ \| |  _| | | | | '_ \| '_ \
     | |  | | (_) | | | | (_) | |_| | | |_| | |_) | | | |
     |_|  |_|\___/|_| |_|\___/ \____|_|\__, | .__/|_| |_|
                              |___/|_|</pre></div>

**MonoGlyph** is a console application for practicing blind typing skills, developed in C++ using the **standard library** exclusively.

---

# Table of Contents

- [Features](#features)
- [Demo](#demo)
- [Build and Installation](#build-and-installation)
- [Quick Start](#quick-start)
- [Font File Format](#font-file-format)
- [Roadmap](#roadmap)
- [Contributing](#contributing)
- [License](#license)

---

# Features

- **Standard Library Only** — No external dependencies; built entirely on C++17 and POSIX APIs.
- **Finite State Machine Architecture** — Clean and modular control flow using a flexible state pattern.
- **Dynamic Terminal UI** — Real-time responsive interface with smooth screen updates using raw ANSI sequences.
- **Custom Font System** — Unicode-aware, vector-based font engine supporting user-defined glyph sets.

---

# Demo

<img src="https://github.com/wqst3/MonoGlyph/blob/master/assets/monoglyph.gif" width="400" alt="Demo GIF">

---

# Build and Installation

## Requirements

* OS: Linux (POSIX-compatible with termios, signalfd, timerfd)
* Compiler: C++17-compatible (e.g. GCC ≥ 7.0, Clang ≥ 5.0)
* Build System: CMake ≥ 3.10
* Dependencies: None (uses only the C++ Standard Library)

## Steps
```bash
# Clone the repository
git clone https://github.com/wqst3/MonoGlyph.git
cd MonoGlyph

# Create a build directory
mkdir build && cd build

# Generate build files (Release)
cmake -DCMAKE_BUILD_TYPE=Release ..

# Or for Debug mode:
cmake -DCMAKE_BUILD_TYPE=Debug ..

# Compile the project
make

# And run it
./MonoGlyph i

# Optionally, install system-wide
sudo make install

# Or specify an install prefix
cmake -DCMAKE_INSTALL_PREFIX=~/usr -DCMAKE_BUILD_TYPE=Release ..
make && make install
```
## Note

The game requires access to the fonts/ directory at runtime.  
Make sure it is located next to the executable (i.e., ./MonoGlyph).

---

## Menu Navigation

* Press i — Start infinite typing mode.
* Press Tab+Enter — Restart the current session.
* Press Esc — Exit to menu or exit the program.

---

# Font File Format

Font files are located in the fonts/ directory (e.g., english.font) and define how each glyph is drawn using normalized vector segments.

## Structure

Each glyph definition consists of:
- A single character (e.g., A, B) on a line by itself.
- A sequence of line segments in the format: x1 y1 x2 y2, where x and y are floats in the range [0.0, 1.0]. Each segment represents a line from (x1, y1) to (x2, y2).


---

## Adding a New Font (Work in Progress)

To add a new font:

1. Create a .font file using the structure above.
2. Place the file inside the fonts/ directory (must be present alongside the executable).
3. Update FontManager::load() in the codebase to register the new font by a unique name.
4. Access the font in code using:

   ```cpp
   fontManager.get("<font_name>");
   ```

⚠️ Support for dynamically loading custom fonts is under development and will be available in a future version.

---

# Roadmap

## Planned Features

- **Multi-language support** — Add support for additional alphabets such as Cyrillic, Greek, and extended Unicode characters.
- **Timed Mode** — A mode where the player types as many glyphs as possible within a fixed time limit.
- **Fixed-Length Mode** — A mode with a set number of glyphs to type, focusing on precision and consistency.
- **Typing Statistics** — Track performance metrics such as score, accuracy, and typing speed.
- **FIGlet-style Font Support** — Render large ASCII art fonts by supporting FIGlet-style font definitions.
- **Error Handling** — Robust runtime checks and graceful fallback mechanisms to ensure stability.
- **Interface Improvements** — Greater dependence on terminal size.
- **Basic Testing** — Core modules are covered by small unit-style tests to ensure correctness.

---

# Contributing

Suggestions and contributions are welcome! Feel free to open issues or create pull requests.

---

# License

This project is licensed under the MIT License.
