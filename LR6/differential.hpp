#pragma once
#include <vector>
#include <functional>

namespace diff
{

	struct vector2f
	{
		vector2f() = default;
		vector2f(float x, float y) : x(x), y(y) { }
		float x;
		float y;
	};

	std::vector<vector2f> methodRungeKuttFirstOrder(
		std::function<float(vector2f)> f,
		float y0,
		float a, float b, float h
	)
	{
		std::vector<vector2f> out_vecs;
		out_vecs.reserve(fabs(a - b) / h);
		out_vecs.emplace_back(a, y0);
		auto x = a + h;
		for (size_t i = 1; x <= b; x += h, i++)
		{
			const auto& last_vec = out_vecs[i - 1];
			const float k = h * f(last_vec);
			const float y = last_vec.y + k;
			out_vecs.emplace_back(x, y);
		}	
		return out_vecs;
	}

	std::vector<vector2f> methodRungeKuttSecondOrder(
		std::function<float(vector2f)> f,
		float y0,
		float a, float b, float h
	)
	{
		std::vector<vector2f> out_vecs;
		out_vecs.reserve(fabs(a - b) / h);
		out_vecs.emplace_back(a, y0);
		auto x = a + h;
		for (size_t i = 1; x <= b; x += h, i++)
		{
			const auto& last_vec = out_vecs[i - 1];
			const float k1 = h * f(last_vec);
			const float k2 = h * f({x, last_vec.y + k1});
			float y = (k1 + k2);
			//fmadd<1, true>(y, last_vec.y);
			out_vecs.emplace_back(x, y);
		}
		return out_vecs;
	}
}
