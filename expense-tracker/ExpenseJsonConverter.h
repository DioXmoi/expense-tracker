#pragma once

#include "Expense.h"
#include "ExpenseUtils.h"

#include <vector>
#include <string>

#include "nlohmann/json.hpp"

using json = nlohmann::json;

namespace ExpenseJsonConverter {
	inline json to_json(const Expense& expense) {
		json json;
		json["ID"] = expense.GetID();
		json["Description"] = expense.GetDescription();
		json["Amount"] = expense.GetAmount();
		json["CreateAt"] = ExpenseUtils::to_string(expense.GetCreateAt());

		return json;
	}

	inline Expense from_json(json& json) {
		ID id;
		std::string description;
		float amount;
		Date createAt;
		json.at("ID").get_to(id);
		json.at("Description").get_to(description);
		json.at("Amount").get_to(amount);
		std::string str;
		json.at("CreateAt").get_to(str);

		createAt = ExpenseUtils::from_string(str);

		return Expense{ id, description, amount, createAt };
	}

	inline std::string Serialize(const std::vector<Expense>& expenses) {
		json json = json::array();
		for (const auto& item : expenses) {
			json.push_back(to_json(item));
		}

		return json.dump(4);
	}

	inline std::vector<Expense> Parse(std::string_view str) {
		if (str.empty()) {
			return { };
		}

		json json = json::parse(str);
		std::vector<Expense> res{ };
		if (json.is_array()) {
			for (auto& item : json) {
				res.emplace_back(from_json(item));
			}
		}

		return res;
	}
}