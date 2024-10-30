#pragma once

#ifndef _14_12_13_10_2024_FILE_H_
#define _14_12_13_10_2024_FILE_H_


#include "Utf8Converter.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <locale>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>


class File {
public:


	explicit File(std::string_view name) 
		: m_name{ name }
	{
        if (!fileExists()) {
            std::wofstream create{ m_name, std::ios::out };
            if (!create.is_open()) {
                throw std::runtime_error{ "File creating error." };
            }
        }
	}


    bool fileExists() const {
        return std::filesystem::exists(m_name);
    }


	std::string read() {
        std::ifstream in{ m_name, std::ios::binary};
        if (!in.is_open()) {
            throw std::runtime_error{ "File opening error." };
        }

        std::string buffer((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());

        if (in.bad()) {
            throw std::ios_base::failure("File reading error.");
        }

        return buffer;
	}


	void write(std::string_view text) {
        std::wofstream out{ m_name, std::ios::binary | std::ios::out };

        out.imbue(std::locale("en_US.UTF-8"));
        if (!out.is_open()) {
            throw std::runtime_error{ "File opening error." };
        }

        std::wstring encode{ Utf8Converter::utf8_decode(text) };

		if (!(out.write(encode.data(), encode.size()))) {
            if (out.bad()) {
                throw std::fstream::failure{ "File writing error: Badbit flag set." };
            }
            else {
                throw std::fstream::failure{ "File writing error: Unknown error." };
            }
		}

        out.flush();
	}


    const std::string& getName() const { return m_name; }

private:

	std::string m_name;
};

#endif // !_14_12_13_10_2024_FILE_H_
