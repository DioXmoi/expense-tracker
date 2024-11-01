#include "ExpenseJsonConverter.h"


json ExpenseJsonConverter::to_json(const Expense& expense) {
	json json;
	json["ID"] = expense.GetID();
	json["Description"] = expense.GetDescription();
	json["Amount"] = expense.GetAmount();
	json["Type"] = ExpenseUtils::to_string(expense.GetType());
	json["CreateAt"] = ExpenseUtils::to_string(expense.GetCreateAt());

	return json;
}


Expense ExpenseJsonConverter::from_json(json& json) {
	ID id;
	std::string description;
	float amount;
	Expense::Type type;
	Date createAt;
	json.at("ID").get_to(id);
	json.at("Description").get_to(description);
	json.at("Amount").get_to(amount);
	std::string str1;
	json.at("Type").get_to(str1);
	std::string str2;
	json.at("CreateAt").get_to(str2);

	type = ExpenseUtils::from_string_type(str1);
	createAt = ExpenseUtils::from_string_date(str2);

	return Expense{ id, description, amount, type, createAt };
}


std::string ExpenseJsonConverter::Serialize(const std::vector<Expense>& expenses) {
	json json = json::array();
	for (const auto& item : expenses) {
		json.push_back(to_json(item));
	}

	return json.dump(4);
}


std::vector<Expense> ExpenseJsonConverter::Parse(std::string_view str) {
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