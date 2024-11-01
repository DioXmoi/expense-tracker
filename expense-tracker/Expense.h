#pragma once


#include <chrono>
#include <string>
#include <string_view>


using ID = int;
using Date = std::chrono::year_month_day;

class Expense {
public:

	enum Type {
		NotInstalled,
		Housing,
		Products,
		Transport,
		Healthcare,
		Clothes,
		Entertainments,
		PersonalExpenses,
		Education,
		Gifts,
		Pets,
		RepairAndMaintenance,
		Other,
	};


	Expense(ID id, std::string_view description, float amount, 
		Type type = DefaultType(),
		Date createAt = DefaultCreateAt());


	ID GetID() const { return m_id; }
	std::string GetDescription() const { return m_description; }
	float GetAmount() const { return m_amount; }
	Type GetType() const { return m_type; }
	Date GetCreateAt() const { return m_createAt; }


	void SetDescription(std::string_view description);
	void SetAmount(float amount);
	void SetType(Type type);


	Expense& operator=(const Expense&) = delete;
	Expense& operator=(Expense&& exp) noexcept;
	Expense(Expense&) = delete;
	Expense(Expense&& exp) noexcept;

private:

	static Date DefaultCreateAt();
	static Type DefaultType();

private:
	ID m_id{};
	std::string m_description{ };
	float m_amount{ };
	Type m_type{ };
	Date m_createAt{ };
};