#pragma once

#include "Expense.h"

#include <string>
#include <string_view>
#include <sstream>


namespace ExpenseUtils {
	inline std::string to_string(const Date& date) {
		std::stringstream buf;

		buf << date;

		return buf.str();
	}

	inline Date from_string(std::string_view str) {
		using namespace std::chrono;

		std::tm t{ };
		std::istringstream stream{ str.data() };
		stream >> std::get_time(&t, "%Y-%m-%d");

		return sys_days{ year{ t.tm_year + 1900 } / (t.tm_mon + 1) / t.tm_mday };
	}
}