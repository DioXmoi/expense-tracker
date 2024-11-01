#pragma once

#include "Expense.h"

#include <string>
#include <vector>


namespace TrackerMenager {

	std::vector<std::string> ParseARGV(int argc, wchar_t* argv[]);

	void HendlerCommend(const std::vector<std::string>& commend, std::vector<Expense>& expenses);
}