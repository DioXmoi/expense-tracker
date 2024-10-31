#pragma once

#include <chrono>
#include <string>
#include <string_view>
#include <utility>


using ID = int;
using Date = std::chrono::year_month_day;

class Expense {
public:
	Expense(ID id, std::string_view description, float amount, Date createAt = std::chrono::floor<std::chrono::days>(std::chrono::system_clock::now()) )
		: m_id{ id }
		, m_description{ description }
		, m_amount{ amount }
		, m_createAt{ createAt }
	{
	}

	ID GetID() const { return m_id; }
	std::string GetDescription() const { return m_description; }
	void SetDescription(std::string_view description) { m_description = description; }
	float  GetAmount() const { return m_amount; }
	void SetAmount(float amount) { m_amount = amount; }
	Date GetCreateAt() const { return m_createAt; }

	Expense& operator=(const Expense&) = delete;

	Expense& operator=(Expense&& exp) noexcept {
		if (&exp != this) {
			m_id = exp.GetID();
			m_description = std::move(exp.GetDescription());
			m_amount = exp.GetAmount();
			m_createAt = std::move(exp.GetCreateAt());
		}

		return *this;
	}

	Expense(Expense&) = delete;

	Expense(Expense&& exp) noexcept {
		m_id = exp.GetID();
		m_description = std::move(exp.GetDescription());
		m_amount = exp.GetAmount();
		m_createAt = std::move(exp.GetCreateAt());
	}

private:
	ID m_id{};
	std::string m_description{ };
	float m_amount{ };
	Date m_createAt{ };
};