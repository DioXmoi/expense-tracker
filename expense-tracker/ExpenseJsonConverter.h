#pragma once

#include "Expense.h"
#include "ExpenseUtils.h"

#include <vector>
#include <string>

#include "nlohmann/json.hpp"

using json = nlohmann::json;

namespace ExpenseJsonConverter {
	json to_json(const Expense& expense);

	Expense from_json(json& json);

	std::string Serialize(const std::vector<Expense>& expenses);

	std::vector<Expense> Parse(std::string_view str);
}