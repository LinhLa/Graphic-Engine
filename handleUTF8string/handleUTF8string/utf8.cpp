#include "stdafx.h"
#include "utf8.h"
#include <clocale>
using namespace std;
utf8::utf8() {}

utf8::~utf8() {}

void utf8::init_from_string(const char* inString)
{
	m_buffer << inString;
}

bool utf8::init_from_file(const char* inPath)
{
	bool bRet = true;
	std::string line;
	m_fstream.open(inPath);
	if (m_fstream.is_open())
	{
		while (getline(m_fstream, line))
		{
			m_buffer << line << '\n';
		}
		m_fstream.close();
	}
	else
	{
		cout << "Unable to open source file";
		bRet = false;
	}
	return bRet;
}

std::vector<std::string> utf8::breakline(size_t max_character_per_line)
{
	std::vector<std::string> stringlist;
	std::string str_temp = m_buffer.str();
	size_t start_byte_pos = 0, next_first_byte_pos = 0;
	uint8_t *p_current_byte = NULL;
	int stt = 1;

	while (next_first_byte_pos < str_temp.length())
	{
		p_current_byte = (uint8_t*)&str_temp[next_first_byte_pos];

		// show log
		std::cout << stt << ".position: " << next_first_byte_pos << " - value:" << int(*p_current_byte) << std::endl;

		// determine next 1st byte of utf8 charater
		next_first_byte_pos = next_utf8_character_pos(next_first_byte_pos, p_current_byte);

		// check if counter the maximum character or reach last line
		if ((stt % max_character_per_line == 0) || next_first_byte_pos >= str_temp.length())
		{
			stringlist.push_back(std::string(&str_temp[start_byte_pos], &str_temp[next_first_byte_pos - 1]));
			start_byte_pos = next_first_byte_pos;
		}
		stt++;
	}

	// show result
	for (auto &ref_string : stringlist)
	{
		for (auto &ref_byte : ref_string)
		{
			printf("0x%x ", uint8_t(ref_byte));
		}
		// break new line
		printf("\n");
	}

	return stringlist;
}

std::string utf8::operator[](size_t pos)
{
	std::string str_ret;
	std::string str_temp = m_buffer.str();
	uint8_t *current_byte_ptr = NULL;
	size_t stt = 1, next_first_byte_pos = 0, first_byte_pos = 0;

	while (first_byte_pos < str_temp.length())
	{
		current_byte_ptr = (uint8_t*)&str_temp[first_byte_pos];

		// show log
		std::cout << stt << ".position: " << first_byte_pos << " - value:" << int(*current_byte_ptr) << std::endl;

		// determine next 1st byte of utf8 charater
		next_first_byte_pos = next_utf8_character_pos(first_byte_pos, current_byte_ptr);

		// check if counter the target character
		if (pos == ++stt)
		{
			str_ret = str_temp.substr(first_byte_pos, next_first_byte_pos - first_byte_pos);
			break;
		}
		first_byte_pos = next_first_byte_pos;
	}

	// show result
	for (auto &i : str_ret)
	{
		printf("%x ", i);
	}

	return str_ret;
}

void utf8::operator<<(const char* utf8string)
{
	m_buffer << utf8string;
}

size_t utf8::next_utf8_character_pos(const size_t &_inpos, const uint8_t const* ptr)
{
	size_t pos = _inpos;
	if ((SixByte & *ptr) == SixByte)
	{
		pos += 6;
	}
	else if ((FiveByte & *ptr) == FiveByte)
	{
		pos += 5;
	}
	else if ((FourByte & *ptr) == FourByte)
	{
		pos += 4;
	}
	else if ((ThreeByte & *ptr) == ThreeByte)
	{
		pos += 3;
	}
	else if ((TwoByte & *ptr) == TwoByte)
	{
		pos += 2;
	}
	else if ((OneByte & *ptr) == OneByte)
	{
		pos += 1;
	}
	return pos;
}

bool utf8::locale_is_utf8()
{
	return (std::string::npos != std::string(std::setlocale(LC_ALL, NULL)).find("UTF - 8"));
}