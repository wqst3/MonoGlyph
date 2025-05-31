cmake_minimum_required(VERSION 3.10)

project(MonoGlyph)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

add_executable(MonoGlyph main.cpp
			 src/terminal.cpp
			 src/MonoGlyph.cpp
			 src/screenBuffer.cpp
			 src/drawer.cpp
			 src/font.cpp
			 src/fontManager.cpp
			 src/fileDescriptor.cpp
		 	 src/eventLoop.cpp
			 src/interfaceDrawer.cpp
		 	 src/stateMenu.cpp
			 src/stateRestart.cpp
			 src/stateInfinite.cpp)

file(COPY ${CMAKE_SOURCE_DIR}/fonts DESTINATION ${CMAKE_BINARY_DIR})

target_compile_options(MonoGlyph PRIVATE -Wall)
