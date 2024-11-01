#include "Expense.h"
#include "ExpenseUtils.h"


#include <utility>
#include <stdexcept>
#include <format>


Expense::Expense(ID id, std::string_view description, float amount, Type type, Date createAt)
	: m_id{ id }
	, m_description{ description }
	, m_amount{ amount }
	, m_type{ type }
	, m_createAt{ createAt }
{
}


void Expense::SetDescription(std::string_view description) {
	if (description.size() > 120 || description.empty()) {
		throw std::invalid_argument(std::format("Invalid argument length cannot be empty or greater than \"{}\"!", 120));
	}

	m_description = description;
}


void Expense::SetAmount(float amount) {
	if (amount < 0) {
		throw std::invalid_argument(std::format("Invalid argument \"{}\" cannot be negative!", amount));
	}

	m_amount = amount;
}


void Expense::SetType(Type type) {
	if (type < Type::NotInstalled || type > Type::Other) {
		throw std::invalid_argument(std::format("Invalid argument \"{}\"!", 
			ExpenseUtils::to_string(type)));
	}

	m_type = type;
}


Expense& Expense::operator=(Expense&& exp) noexcept {
	if (&exp != this) {
		m_id = exp.GetID();
		m_description = std::move(exp.GetDescription());
		m_amount = exp.GetAmount();
		m_type = exp.GetType();
		m_createAt = std::move(exp.GetCreateAt());
	}

	return *this;
}


Expense::Expense(Expense&& exp) noexcept {
	m_id = exp.GetID();
	m_description = std::move(exp.GetDescription());
	m_amount = exp.GetAmount();
	m_type = exp.GetType();
	m_createAt = std::move(exp.GetCreateAt());
}


Date Expense::DefaultCreateAt() {
	return std::chrono::floor<std::chrono::days>(std::chrono::system_clock::now());
}

Expense::Type Expense::DefaultType() {
	return Type::NotInstalled;
}