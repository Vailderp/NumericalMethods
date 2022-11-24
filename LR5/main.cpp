#include <iostream>
#include <functional>
#include "couter.hpp"
#include "integrator.hpp"

//const _Smanip& _Manip

int main()
{
	//Variant 5
	{
		constexpr float a = 4;
		constexpr float b = 5;
		constexpr float h = 0.0001;
		std::function<float(float)> f = [](const float x) -> float {
			return x * sqrtf(powf(x, 2) - 16);
		};

		cn::Table<20, 20> table;
		table.SetHeader("Variant 5");
		table.SetColumnNames("Method", "S");
		table.AddRow("Rectangle Left", inpr::rectangleLeft(f, a, b, h));
		table.AddRow("Rectangle Central", inpr::rectangleCentral(f, a, b, h));
		//table.AddRow("Trapezoid", inpr::trapezoid(f, a, b, h));
		//table.AddRow("Simpson", inpr::simpson(f, a, b));
		std::cout << table;
	}

	//Variant 9
	{
		constexpr float a = 0;
		constexpr float b = 18;
		constexpr float h = 0.0001;
		std::function<float(float)> v = [](const float t) -> float {
			return 4.5f * powf(t, 2) + 5.0f * t;
		};

		cn::Table<20, 20> table;
		table.SetHeader("Variant 9");
		table.SetColumnNames("Method", "S");
		//table.AddRow("Rectangle Left", inpr::rectangleLeft(v, a, b, h));
		//table.AddRow("Rectangle Central", inpr::rectangleCentral(v, a, b, h));
		table.AddRow("Trapezoid", inpr::trapezoid(v, a, b, h));
		table.AddRow("Simpson", inpr::simpson(v, a, b));
		std::cout << table;
	}

	//Variant 14
	{
		constexpr float a = 0;
		constexpr float b = 5;
		constexpr float h = 0.0001;
		std::function<float(float)> v1 = [](const float t) -> float {
			return 6.0f * powf(t, 2) + 2.0f * t;
		};
		std::function<float(float)> v2 = [](const float t) -> float {
			return 4.0f * t + 5.0f;
		};

		cn::Table<20, 20> table;
		table.SetHeader("Variant 14");
		table.SetColumnNames("Method", "S");

		const float trapezoid1 = inpr::trapezoid(v1, a, b, h);
		const float simpson1 = inpr::simpson(v1, a, b);

		const float trapezoid2 = inpr::trapezoid(v2, a, b, h);
		const float simpson2 = inpr::simpson(v2, a, b);

		const float S1 = (trapezoid1 + simpson1) * 0.50f;
		const float S2 = (trapezoid2 + simpson2) * 0.50f;

		table.AddRow("Trapezoid v1", trapezoid1);
		table.AddRow("Simpson v1", simpson1);

		table.AddRow("Trapezoid v2", trapezoid2);
		table.AddRow("Simpson v2", simpson2);

		table.AddRow("Averrage v1", S1);
		table.AddRow("Averrage v2",S2);

		table.AddRow("Distance", fabs(S1 - S2));

		std::cout << table;
	}

	return 0;
}
