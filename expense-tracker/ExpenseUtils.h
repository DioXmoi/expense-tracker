#pragma once

#include "Expense.h"


#include <string>
#include <string_view>


namespace ExpenseUtils {
	std::string to_string(const Date& date);

	Date from_string_date(std::string_view str);

	std::string to_string(Expense::Type type);

	Expense::Type from_string_type(std::string_view str);
}