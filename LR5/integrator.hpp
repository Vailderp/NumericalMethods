#pragma once
#include <functional>

namespace inpr 
{
	inline float rectangleLeft(
		std::function<float(float)> f, 
		const float a, 
		const float b, 
		const float h
	)
	{
		float integral = .0f;
		for (float x = a; x <= b; x += h)
			integral += f(x) * h;
		return integral;
	}

	inline float rectangleCentral(
		std::function<float(float)> f,
		const float a,
		const float b,
		const float h
	)
	{
		float integral = .0f;
		for (float x = a; x <= b; x += h)
			integral += f(x + h / 2.0f) * h;
		return integral;
	}

	void fmul2(float& single)
	{
		*(__int32*)&single += 0b0'00000001'00000000000000000000000l;
	}

	void fdiv2(float& single)
	{
		*(__int32*)&single -= 0b0'00000001'00000000000000000000000l;
	}

	void fmul4(float& single)
	{
		*(__int32*)&single += 0b0'00000010'00000000000000000000000l;
	}

	inline float trapezoid(
		std::function<float(float)> f,
		const float a,
		const float b,
		const float h
	)
	{
		const float y0 = f(a), yn = f(b);
		float integral = .0f;
		for (float x = a + h; x <= b - h; x += h)
			integral += f(x);
		fmul2(integral);
		integral = h * (y0 + integral + yn);
		fdiv2(integral);
		return integral;
	}

	inline float simpson(
		std::function<float(float)> f,
		const float a,
		const float b
	)
	{
		const float M = (b - a) / 6.0f;
		float a_b = a + b;
		fdiv2(a_b);

		float T = f(a_b);
		fmul4(T);
		const float y0 = f(a), yn = f(b);
		const float integral = M * (y0 + T + yn);
		return integral;
	}
}
