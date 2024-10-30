#pragma once

#ifndef _18_23_17_10_2024_UTF8CONVERTER_H_

#define _18_23_17_10_2024_UTF8CONVERTER_H_


#include <string>
#include <string_view>
#include <Windows.h>


namespace Utf8Converter {
    // Convert a wide Unicode string to an UTF8 string
    inline std::string utf8_encode(std::wstring_view wstr) {
        if (wstr.empty()) {
            return std::string();
        }

        int size_needed = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), nullptr, 0, nullptr, nullptr);
        std::string strTo(size_needed, 0);
        WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), &strTo[0], size_needed, nullptr, nullptr);

        return strTo;
    }

    // Convert an UTF8 string to a wide Unicode String
    inline std::wstring utf8_decode(std::string_view str) {
        if (str.empty()) {
            return std::wstring();
        }

        int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), nullptr, 0);
        std::wstring wstrTo(size_needed, 0);
        MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);

        return wstrTo;
    }
}

#endif // !_18_23_17_10_2024_UTF8CONVERTER_H_
