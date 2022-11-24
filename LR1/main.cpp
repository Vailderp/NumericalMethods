#include <iostream>
#include <iomanip>
#include <functional>
#include <equations>

int main()
{
	using namespace std;

	cn::Table<25, 16> table;
	table.AddRows(5);

	// Variant 5: x^4 - x^3 + 15x^2 - 30 = 0
	{
		constexpr float a = .0f, b = 4.f;
		auto f = [](const float x) {
			return powf(x, 4) - powf(x, 3) + 15 * pow(x, 2) - 30;
		};
		table.SetHeader("5 variant: x^4 - x^3 + 15x^2 - 30 = 0");
		table.SetColumnNames("Method name", "Value of x");
		table.SetRow(0, "Brute force", ChMethods::bruteForce(f, a, b));
		table.SetRow(1, "Dichotomy", ChMethods::dichotomy(f, a, b));
		table.SetRow(2, "Golden section", ChMethods::goldenSection(f, a, b));
		table.SetRow(3, "Chords", ChMethods::chords(f, a, b));
		float x = ChMethods::chords(f, a, b, 0.1);
		table.SetRow(4, "Newton", ChMethods::newton(f, x));
		cout << table;
	}

	// Variant 8: h(t) = -4t^2 + 7t + 2.04
	{
		constexpr float a = .0f, b = 3.f;
		auto f = [](const float t) {
			return -4.f * pow(t, 2) + 7.f * t + 2.04f;
		};
		table.SetHeader("8 variant: h(t) = -4t^2 + 7t + 2.04");
		table.SetColumnNames("Method name", "Value of t");
		table.SetRow(0, "Brute force", ChMethods::bruteForce(f, a, b));
		table.SetRow(1, "Dichotomy", ChMethods::dichotomy(f, a, b));
		table.SetRow(2, "Golden section", ChMethods::goldenSection(f, a, b));
		table.SetRow(3, "Chords", ChMethods::chords(f, a, b));
		float x = ChMethods::chords(f, a, b, 0.1);
		table.SetRow(4, "Newton", ChMethods::newton(f, x));
		cout << table;
	}

	// Variant 12: U(t) = 30 * cos(0.2t + 1,2)
	{
		constexpr float a = .0f, b = 14.6f;
		auto f = [](const float t) {
			return 30.f * std::cosf(0.2f * t + 1.2f);
		};
		table.SetHeader("12 variant: U(t) = 30 * cos(0.2t + 1,2)");
		table.SetColumnNames("Method name", "Value of t");
		table.SetRow(0, "Brute force", ChMethods::bruteForce(f, a, b));
		table.SetRow(1, "Dichotomy", ChMethods::dichotomy(f, a, b));
		table.SetRow(2, "Golden section", ChMethods::goldenSection(f, a, b));
		table.SetRow(3, "Chords", ChMethods::chords(f, a, b));
		float x = ChMethods::chords(f, a, b, 0.1);
		table.SetRow(4, "Newton", ChMethods::newton(f, x));
		cout << table;
	}

	// Variant 14: * H0 = 40, k = 1/50
	{
		constexpr float a = .0f, b = 200.f;
		constexpr float g = 9.8f;
		constexpr float H0 = 40.f, k = 1.f / 50.f;
		auto f = [](const float t) {
			return H0 - k * t * std::sqrtf(2.0f * g * H0) + (g * k * k * t * t) / 2.f;
		};
		table.SetHeader("14 variant: * H0 = 40, k = 1/50");
		table.SetColumnNames("Method name", "Value of t");
		table.SetRow(0, "Brute force", ChMethods::bruteForce(f, a, b));
		table.SetRow(1, "Chords", ChMethods::chords(f, a, b));
		float x = ChMethods::chords(f, a, b, 0.1);
		table.SetRow(2, "Newton", ChMethods::newton(f, ChMethods::goldenSection(f, a, b, x)));
		table.SetRowsOutputLimit(3);
		cout << table << std::endl;
	}

	return 0;
}
