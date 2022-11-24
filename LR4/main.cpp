#include <iostream>
#include <functional>
#include "couter.hpp"

#define __EQUATIONS_H__
#include "equations.hpp"

#include "slau.hpp"

#include "interpollator.hpp"

//const _Smanip& _Manip

int main()
{
	using namespace std;

	//Variant 4
	{
		intr::points_vector_t points =
		{
			{0, -12}, {50, -4.93}, {100, -2}, {150, 0.25}
		};

		for (auto& vec : points)
		{
			//std::cout << "(" << vec.x << ";" << vec.y << ") ";
		}

		//std::cout << std::endl;		std::cout << std::endl;
		float h = 0.5f;
		for (float arg = 0; arg < 150; arg += h)
		{
			intr::vector2f_t point = intr::LagrangeMethod::compute(points, arg);
			//std::cout << "(" << point.x << ";" << point.y << ") ";
		}
	}

	//Variant 9
	{
		const float a = 0;
		const float b = 4;
		const float h = 1;
		intr::points_vector_t points = intr::createGraph(a, b, h, [](const float x) -> float
			{
				return 1 / (1 + powf(std::numbers::e_v<float>, -x));
			}
		);

		//intr::NewtonMethod* newton = new intr::NewtonMethodUniformed;
		intr::NewtonMethod* newton = new intr::NewtonMethodNonUniformed;

		newton->bind(points);
		auto vecs = newton->computePoints(0.20);

		for (auto& vec : points)
		{
			//std::cout << "(" << vec.x << ";" << vec.y << ") ";
		}

		//std::cout << std::endl;		std::cout << std::endl;

		for (auto& vec : vecs)
		{
			//std::cout << "(" << vec.x << ";" << vec.y << ") ";
		}
	}

	//Variant 15
	{
		intr::points_vector_t points =
		{
			{1, 11}, {2, 12}, {3, 13}, {4, 14}, {5, 15}, 
			{6, 16}, {7, 17}, {8, 18}, {9, 19}, {10, 20}, 
			{11, 19}, {12, 18}, {13, 17}, {14, 16}, {15, 20}, 
			{16, 17}, {17, 16}, {18, 17}, {19, 14}, {20, 16}, 
			{21, 18}, {22, 21}, {23, 20}, {24, 16}, {25, 18},
			{26, 19}, {27, 22}, {28, 24}, {29, 26}, {30, 28},
		};

		auto vecs = intr::SplineParabolicMethod::computePoints(points, 0.1);
		for (auto& vec : points)
			std::cout << "(" << vec.x << ";" << vec.y << ") ";

		std::cout << std::endl;		std::cout << std::endl;

		for (auto& vec : vecs)
			std::cout << "(" << vec.x << ";" << vec.y << ") ";
	}


	return 0;
}
