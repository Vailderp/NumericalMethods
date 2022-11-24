//CXX20
#include <functional>
#include <numbers>
inline float bruteForce(
		const std::function<float(float)>& f,
		const float a, 
		const float b, 
		const float epsilon = .0001f
	)
	{
		float x_min = a;
		float y_min_abs = fabs(f(a));
		for (float x = a + epsilon; x < b; x += epsilon)
		{
			float y = fabs(f(x));
			if (y < y_min_abs)
			{
				x_min = x;
				y_min_abs = y;
			}
		}
		return x_min;
	}

	inline float dichotomy(
		const std::function<float(float)>& f,
		float a,
		float b,
		const float epsilon = .0001f
	)
	{
		float x = 0;
		float y = 0;
		do
		{
			x = (a + b) / 2.0f;
			y = f(x);
			if (f(a) * y < 0)
			{
				b = x;
			}
			else
			{
				a = x;
			}
		} while (fabs(y) > epsilon);
		return x;
	}

	inline float goldenSection(
		const std::function<float(float)>& f,
		float a,
		float b,
		const float epsilon = .0001f
	)
	{
		float x = 0;
		float y = 0;
		do
		{
			x = a + (b - a) / std::numbers::phi_v<float>;
			y = f(x);
			if (f(a) * y < 0)
			{
				b = x;
			}
			else
			{
				a = x;
			}
		} while (fabs(y) > epsilon);
		return x;
	}

	inline float chords(
		const std::function<float(float)>& f,
		float a,
		float b,
		const float epsilon = .0001f
	)
	{
		float x = 0;
		float y = 0;
		do
		{
			x = -(f(a) * (b - a)) / (f(b) - f(a)) + a;
			y = f(x);
			if (f(a) * y < 0)
			{
				b = x;
			}
			else
			{
				a = x;
			}
		} while (fabs(y) > epsilon);
		return x;
	}

	inline float newton(
		const std::function<float(float)>& f,
		float x,
		const float epsilon = .0001f
	)
	{
		float derivative = (f(x + epsilon) - f(x)) / epsilon;
		do
		{
			x -= f(x) / derivative;
		} while (fabs(f(x)) > epsilon);
		return x;
	}
