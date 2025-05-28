#pragma once

#include "commonTypes.h"

#include <vector>
#include <unordered_map>
#include <string>
#include <filesystem>

struct Glyph
{
	std::vector<Vector<float>> segments;
};

class Font
{
	using Line = Vector<float>;
	using Size = Point<int>;

	std::string name_;
	std::unordered_map<char, Glyph> glyphs_;

	void parseFontFile(const std::filesystem::path&);

public:
	explicit Font(const std::filesystem::path&);

	const std::string getLetters() const noexcept;

	const Glyph& get(char) const;
	const Glyph* getIfExists(char) const noexcept;

	const std::string name() const noexcept { return name_; }
};

