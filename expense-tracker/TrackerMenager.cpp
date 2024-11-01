#include "TrackerMenager.h"
#include "Utf8Converter.h"
#include "ExpenseUtils.h"

#include <array>
#include <chrono>
#include <iostream>
#include <stdexcept>
#include <string_view>
#include <format>
#include <numeric>

enum MonthTypes {
	January,
	February,
	March,
	April,
	May,
	June,
	July,
	August,
	September,
	October,
	November,
	December,
};

enum CommendTypes {
	Add,
	Update,
	Delete,
	List,
	Summary,
};



std::vector<std::string> TrackerMenager::ParseARGV(int argc, wchar_t* argv[]) {
	std::vector<std::string> commends{};
	for (int i{ 1 }; i < argc; ++i) {
		commends.emplace_back(Utf8Converter::utf8_encode(argv[i]));
	}

	return commends;
}



CommendTypes ParseCommendTypes(std::string_view str);
void HendlerCommendAdd(const std::vector<std::string>& commend, std::vector<Expense>& expenses);
void HendlerCommendUpdate(const std::vector<std::string>& commend, std::vector<Expense>& expenses);
void HendlerCommendDelete(const std::vector<std::string>& commend, std::vector<Expense>& expenses);
void HendlerCommendList(const std::vector<std::string>& commend, std::vector<Expense>& expenses);
void HendlerCommendSummary(const std::vector<std::string>& commend, std::vector<Expense>& expenses);

void TrackerMenager::HendlerCommend(const std::vector<std::string>& commend, std::vector<Expense>& expenses) {
	if (commend.empty()) {
		throw std::runtime_error("Error:\tArgument commend is empty!");
	}

	CommendTypes type{ ParseCommendTypes(commend.front()) };
	switch (type) {
		case CommendTypes::Add: 
			HendlerCommendAdd(commend, expenses); 
			break;
		case CommendTypes::Update:
			HendlerCommendUpdate(commend, expenses);
			break;
		case CommendTypes::Delete:
			HendlerCommendDelete(commend, expenses);
			break;
		case CommendTypes::List:
			HendlerCommendList(commend, expenses);
			break;
		case CommendTypes::Summary:
			HendlerCommendSummary(commend, expenses);
			break;
	}
}

CommendTypes ParseCommendTypes(std::string_view str) {
	static constexpr std::array names = { "add", "update", "delete", "list", "summary" };

	auto found{ std::ranges::find(names, str) };
	if (found == names.end()) {
		throw std::invalid_argument(std::format("Error:\tinvalide argument - \"{}\"!", str));
	}

	return static_cast<CommendTypes>(found - names.begin());
}



static void IsValideCountArgument(const std::vector<std::string>& commend, std::size_t count) {
	if (commend.size() != count) {
		throw std::invalid_argument("Error:\tMissing arguments for command!");
	}
}

static std::string_view GetValideDescription(const std::vector<std::string>& commend, std::size_t index) {
	if (commend[index] != "--description") {
		throw std::invalid_argument(std::format("Error:\tinvalide argument - \"{}\"! {}",
			commend[index], "You should use \"--description\"!"));
	}

	return commend[index + 1]; //the index + 1 is the value of the key in the array by index
}

static float GetValideAmount(const std::vector<std::string>& commend, std::size_t index) {
	if (commend[index] != "--amount") {
		throw std::invalid_argument(std::format("Error:\tinvalide argument - \"{}\"! {}",
			commend[index], "You should use \"--amount\"!"));
	}

	float amount;
	try {
		amount = static_cast<float>(std::stoi(commend[index + 1])); //the index + 1 is the value of the key in the array by index
	}
	catch (std::invalid_argument&) {
		throw std::invalid_argument(std::format("You have entered \"{}\" and you needed a number!", commend[index + 1]));
	}

	return amount;
}

static ID GetValidID(const std::vector<std::string>& commend, std::size_t index) {
	if (commend[index] != "--id") {
		throw std::invalid_argument(std::format("Error:\tinvalide argument - \"{}\"! {}",
			commend[index], "You should use \"--id\"!"));
	}

	ID id;
	try {
		id = static_cast<ID>(std::stoi(commend[index + 1])); //the index + 1 is the value of the key in the array by index
	}
	catch (std::invalid_argument&) {
		throw std::invalid_argument(std::format("You have entered \"{}\" and you needed a number!", commend[index + 1]));
	}

	return id;
}

static MonthTypes GetValidMonth(const std::vector<std::string>& commend, std::size_t index) {
	if (commend[index] != "--month") {
		throw std::invalid_argument(std::format("Error:\tinvalide argument - \"{}\"! {}",
			commend[index], "You should use \"--month\"!"));
	}

	MonthTypes month;
	try {
		month = static_cast<MonthTypes>(std::stoi(commend[index + 1]) - 1); //the index + 1 is the value of the key in the array by index
	}
	catch (std::invalid_argument&) {
		throw std::invalid_argument(std::format("You have entered \"{}\" and you needed a number!", commend[index + 1]));
	}

	if (MonthTypes::January > month || month > MonthTypes::December) {
		throw std::invalid_argument(std::format("Error:\tinvalide argument - \"{}\"! {}",
			static_cast<int>(month), "You should use \"1 - 12\"!"));
	}

	return month;
}

static Expense::Type GetValidExpenseType(const std::vector<std::string>& commend, std::size_t index) {
	if (commend[index] != "--type") {
		throw std::invalid_argument(std::format("Error:\tinvalide argument - \"{}\"! {}",
			commend[index], "You should use \"--type\"!"));
	}

	//the index + 1 is the value of the key in the array by index
	return ExpenseUtils::from_string_type(commend[index + 1]);
}

static auto FindOrderByID(std::vector<Expense>& expenses, ID id) {
	auto found{ std::ranges::find_if(expenses,
		[id](const auto& item) -> bool { return item.GetID() == id; }) };
	if (found == expenses.end()) {
		throw std::runtime_error(
			std::format("Error:\tThe object with id - \"{}\" was not found!", id));
	}

	return found;
}

static void PrintAll(const std::vector<Expense>& expenses) {
	std::wcout.imbue(std::locale("en_US.UTF-8"));
	for (const auto& item : expenses) {
		std::wcout << std::format(L"ID: {}\tDATE: {}\tDESCRIPTION: {}\tAMOUNT: {}\tTYPE: {}\n",
			item.GetID(),
			Utf8Converter::utf8_decode(ExpenseUtils::to_string(item.GetCreateAt())),
			Utf8Converter::utf8_decode(item.GetDescription()),
			item.GetAmount(),
			Utf8Converter::utf8_decode(ExpenseUtils::to_string(item.GetType())));
	}
}

static void Print(const std::vector<Expense>& expenses, Expense::Type type) {
	std::wcout.imbue(std::locale("en_US.UTF-8"));
	for (const auto& item : expenses) {
		if (item.GetType() == type) {
			std::wcout << std::format(L"ID: {}\tDATE: {}\tDESCRIPTION: {}\tAMOUNT: {}\n",
				item.GetID(),
				Utf8Converter::utf8_decode(ExpenseUtils::to_string(item.GetCreateAt())),
				Utf8Converter::utf8_decode(item.GetDescription()),
				item.GetAmount());
		}
	}
}



void HendlerCommendAdd(const std::vector<std::string>& commend, std::vector<Expense>& expenses) {
	IsValideCountArgument(commend, 5);
	std::string_view description{ GetValideDescription(commend, 1) };
	float amount{ GetValideAmount(commend, 3) };
	Expense::Type type{ };

	ID id{ 1 };
	if (!expenses.empty()) {
		id = expenses.back().GetID() + 1;
	}

	expenses.emplace_back(Expense{ id, description, amount, type });
	std::cout << std::format("Expense added successfully.(ID: {})\n", id);
}

void HendlerCommendUpdate(const std::vector<std::string>& commend, std::vector<Expense>& expenses) {
	IsValideCountArgument(commend, 5);

	ID id{ GetValidID(commend, 3) };
	auto found{ FindOrderByID(expenses, id) };
	try {
		found -> SetDescription(GetValideDescription(commend, 1));
	}
	catch (std::invalid_argument&) {
		try {
			found -> SetAmount(GetValideAmount(commend, 1));
		}
		catch (std::invalid_argument&) {
			found -> SetType(GetValidExpenseType(commend, 1));

		}
	}

	std::cout << std::format("Expense updated successfully.(ID: {})\n", id);
}

void HendlerCommendDelete(const std::vector<std::string>& commend, std::vector<Expense>& expenses) {
	IsValideCountArgument(commend, 3);
	if (expenses.empty()) {
		throw std::runtime_error("Error:\tEmpty list expenses!");
	}

	ID id{ GetValidID(commend, 1) };
	auto found{ FindOrderByID(expenses, id) };
	
	expenses.erase(found);
	std::cout << std::format("Expense deleted successfully.(ID: {})\n", id);
}

void HendlerCommendList(const std::vector<std::string>& commend, std::vector<Expense>& expenses) {
	if (expenses.empty()) {
		std::cout << "List is empty!\n";
	}

	if (commend.size() == 1) {
		PrintAll(expenses);
		return;
	}

	IsValideCountArgument(commend, 3);
	Print(expenses, GetValidExpenseType(commend, 1));
}

void HendlerCommendSummary(const std::vector<std::string>& commend, std::vector<Expense>& expenses) {
	if (commend.size() == 1) {
		float sum{ std::accumulate(expenses.begin(), expenses.end(), 0.0F,
			[](float acc, const auto& item) -> float { return acc += item.GetAmount(); }) };
		std::cout << std::format("Total expenses: {}\n", sum);
		return;
	}

	IsValideCountArgument(commend, 3);
	MonthTypes type{ GetValidMonth(commend, 1) };
	float sum{ std::accumulate(expenses.begin(), expenses.end(), 0.0F,
			[type](float acc, const auto& item) -> float {
				std::chrono::month month{ item.GetCreateAt().month() };
				return month == static_cast<std::chrono::month>(static_cast<int>(type) + 1) ? 
					acc + item.GetAmount() : acc; }) };

	std::cout << std::format("Total expenses: {}.\tPer month {}\n", sum, static_cast<int>(type) + 1);
}