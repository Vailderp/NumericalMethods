#include <iostream>
#include <functional>
#include "couter.hpp"

#define __EQUATIONS_H__
#include "equations.hpp"

#include "slau.hpp"

//const _Smanip& _Manip

int main()
{
	using namespace std;

	// Variant 3
	{
		cn::Table<11, 11, 11> table;
		table.SetHeader("Variant 3");
		table.SetColumnNames("X1", "X2", "X3");

		slau::matrixf_t matrix = {
		{ 2, 5, 3 },
		{ 4, 4, 2 },
		{ 4, 6, 5 }
		};

		slau::rowf_t y = { -25, -26, -30 };

		auto x = slau::Gauss::computeLinnearSystem(matrix, y);

		table.AddRow(x[0], x[1], x[2]);

		std::cout << table;
	}

	// Variant 8
	{
		cn::Table<11, 11, 11> table;
		table.SetHeader("Variant 8");
		table.SetColumnNames("X1", "X2", "X3");

		slau::matrixf_t matrix = {
		{ 5, 20, 6 },
		{ 6, 14, 43 },
		{ 10, 19, 21 }
		};

		slau::rowf_t y = { 232, 449, 359 };

		auto x = slau::Gauss::computeLinnearSystem(matrix, y);

		table.AddRow(x[0], x[1], x[2]);

		std::cout << table;
	}

	// Variant 12
	{
		cn::Table<11, 11, 11> table;
		table.SetHeader("Variant 12");
		table.SetColumnNames("X1", "X2", "X3");

		slau::matrixf_t matrix = {
		{ 7, 2, 1 },
		{ 7, 3, 2 },
		{ 5, 2, 2 }
		};

		slau::rowf_t y = { 71, 93, 64 };

		auto x = slau::Gauss::computeLinnearSystem(matrix, y);

		table.AddRow(x[0], x[1], x[2]);

		std::cout << table;
	}

	// Variant 14
	{
		cn::Table<11, 11> table;
		table.SetHeader("Variant 12");
		table.SetColumnNames("X1", "X2");

		slau::matrixf_t matrix = {
		{ 1, -2 },
		{ 3, 5 }
		};

		slau::rowf_t y = { -1, 8 };

		auto x = slau::Gauss::computeLinnearSystem(matrix, y);

		table.AddRow(x[0], x[1]);

		std::cout << table;
	}

	// Variant 15
	{
		cn::Table<11, 11, 11> table;
		table.SetHeader("Variant 15");
		table.SetColumnNames("X1", "X2", "X3");

		slau::matrixf_t matrix = {
		{ 1, -2, -2.8 },
		{ 3, 5, -2.8 },
		{ 4, 3, -2.8 }
		};

		slau::rowf_t y = { -3.8, 8, 7 };

		auto x = slau::Gauss::computeLinnearSystem(matrix, y);

		table.AddRow(x[0], x[1], x[2]);

		std::cout << table;
	}

	return 0;
}
