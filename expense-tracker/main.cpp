#include "File.h"
#include "TrackerMenager.h"
#include "Expense.h"
#include "ExpenseJsonConverter.h"

#include <iostream>
#include <vector>


int wmain(int argc, wchar_t* argv[]) {
	setlocale(0, "");

	File file{ "Expenses.json" };
	std::vector expenses{ ExpenseJsonConverter::Parse(file.read()) };

	try {
		std::vector commend{ TrackerMenager::ParseARGV(argc, argv) };
		TrackerMenager::HendlerCommend(commend, expenses);
	}
	catch (std::exception& exception) {
		std::cout << exception.what() << "\n";
	}

	file.write(ExpenseJsonConverter::Serialize(expenses));

	return 0;
}
