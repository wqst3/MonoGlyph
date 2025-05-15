#pragma once

#include "commonTypes.h"
#include "font.h"

#include <vector>
#include <unordered_map>
#include <memory>
#include <string>
#include <filesystem>

class FontManager
{
	std::filesystem::path fontsDir_;
	std::unordered_map<std::string, std::shared_ptr<Font>> fonts_;

	std::vector<std::filesystem::path> findFontFiles() const;

public:
	explicit FontManager(const std::filesystem::path& = "./fonts");

	void load();

	std::vector<std::string> listFontNames() const;
	std::shared_ptr<Font> get(const std::string&) const;

	bool has(const std::string&) const noexcept;
};

