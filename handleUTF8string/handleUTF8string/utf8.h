#pragma once
#include <cstdint>
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
class utf8
{
public:
	utf8();

	~utf8();

	void init_from_string(const char* inString);

	bool init_from_file(const char* inPath);

	std::vector<std::string> breakline(size_t max_character_per_line);

	std::string operator[](size_t pos);
	void operator<<(const char* utf8string);

private:
	std::ifstream m_fstream;
	std::ostringstream m_buffer;

	const uint8_t OneByte{ 0B0000'0000 };		// represents 0
	const uint8_t TwoByte{ 0B1100'0000 };		// represents 110xxxxx
	const uint8_t ThreeByte{ 0B1110'0000 };		// represents 1110xxxx
	const uint8_t FourByte{ 0B1111'0000 };		// represents 11110xxx
	const uint8_t FiveByte{ 0B1111'1000 };		// represents 111110xx
	const uint8_t SixByte{ 0B1111'1100 };		// represents 1111110x
private:
	size_t next_utf8_character_pos(const size_t &_inpos, const uint8_t const* ptr);
	bool locale_is_utf8();
};