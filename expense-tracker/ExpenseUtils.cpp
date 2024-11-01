#include "ExpenseUtils.h"


#include <algorithm>
#include <array>
#include <sstream>
#include <format>



std::string ExpenseUtils::to_string(const Date& date) {
	std::stringstream buf;

	buf << date;

	return buf.str();
}

Date ExpenseUtils::from_string_date(std::string_view str) {
	using namespace std::chrono;

	std::tm t{ };
	std::istringstream stream{ str.data() };
	stream >> std::get_time(&t, "%Y-%m-%d");

	return sys_days{ year{ t.tm_year + 1900 } / (t.tm_mon + 1) / t.tm_mday };
}

std::string ExpenseUtils::to_string(Expense::Type type) {
	constexpr std::array name{ "not installed", "housing", "products", "transport", "healthcare", "clothes",
		"entertainments", "personalExpenses", "education", "gifts", "pets", "pepair and maintenance", "other" };

	return name[type];
}

Expense::Type ExpenseUtils::from_string_type(std::string_view str) {
	constexpr std::array name{ "not installed", "housing", "products", "transport", "healthcare", "clothes",
		"entertainments", "personalExpenses", "education", "gifts", "pets", "pepair and maintenance", "other" };

	auto found{ std::ranges::find(name, str) };
	if (found == name.end()) {
		throw std::invalid_argument(std::format("Error:\tinvalide argument - \"{}\"! {}",
			str, "You should use \"Not Installed, Housing, Products, Transport, Healthcare, Clothes, Entertainments, Personal Expenses, Education, Gifts, Pets, Repair And Maintenance or Other.\"!"));

	}

	return static_cast<Expense::Type>(found - name.begin());
}