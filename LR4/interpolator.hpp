#pragma once
#include <vector>
#include <algorithm>
#include <stdexcept>
#include <functional>
namespace intr
{
	class vector2f_t;
	using points_vector_t = std::vector<vector2f_t>;

	class vector2f_t
	{
	public:
		vector2f_t() : 
			x(.0f), y(.0f)
		{ }
		vector2f_t(const float x, const float y) :
			x(x), y(y)
		{ }
		vector2f_t(const vector2f_t& src) :
			x(src.x), y(src.y)
		{ }
		float x;
		float y;
	};

	inline void getMinMaxX(const points_vector_t& points, mutable vector2f_t& out)
	{
		out = { points[0].x, points[0].x };
		for (decltype(auto) point : points)
		{
			if (point.x > out.x) out.x = point.x;
			else if (point.y < out.x) out.y = point.x;
		}
	}

	static points_vector_t createGraph(
		const float x_left, const float x_right,
		const float h, std::function<float(float)> f
	)
	{
		points_vector_t out_points;
		for (float x = x_left; x <= x_right; x++)
			out_points.emplace_back(x, f(x));
		return out_points;
	}

	inline float fmadd(const float single0, const float single1, const float single2)
	{
		return single0 * single1 + single2;
	}

	class NewtonMethod
	{
	public:
		virtual void			bind(const points_vector_t& points_) = 0;
		virtual vector2f_t		computeVector(const float x) = 0;
		virtual points_vector_t computePoints(const float h) = 0;
	};
	
	class NewtonMethodUniformed : public NewtonMethod
	{
	public:
		NewtonMethodUniformed() :
			n(0) 
		{}

		NewtonMethodUniformed(const points_vector_t& points_)
		{
			this->bind(points_);
		}

		void bind(const points_vector_t& points_) override
		{
			this->points = points_;
			if (points.size() < 2) throw std::runtime_error("vector of points is empty");
			n = points.size();
			dxi.resize(n);
			dny.resize(n);
			ak.resize(n);

			getMinMaxX(points, max_x1_x2);

			for (size_t i = 0; i < n - 1; i++)
				dny[0].push_back(points[i + 1].y - points[i].y);

			for (size_t dn = 1; dn < n - 1; dn++)
				for (size_t i = 1; i < n - dn; i++)
					dny[dn].push_back(dny[dn - 1][i] - dny[dn - 1][i - 1]);

			ak[0] = points[0].y;
			for (size_t k = 1; k < n; k++)
				ak[k] = dny[k - 1][0] / (tgammaf(k + 1) * powf(points[k].x - points[k - 1].x, k));
		}

		vector2f_t computeVector(const float x) override
		{
			vector2f_t out_vec = { x, 0 };

			dxi[0] = x - points[0].x;
			for (size_t i = 1; i < n; i++)
				dxi[i] = (x - points[i].x) * dxi[i - 1];

			out_vec.y = ak[0];
			for (size_t i = 1; i < n; i++)
				out_vec.y = fmadd(ak[i], dxi[i - 1], out_vec.y);

			return out_vec;
		}

		points_vector_t computePoints(const float h) override
		{
			if (ak.empty()) throw std::runtime_error("class NewtonMethod is not initialized");
			points_vector_t out_points;
			for (float x = points.front().x; x <= points.back().x + h; x += h)
				out_points.push_back(computeVector(x));
			return out_points;
		}

	private:
		size_t n;
		std::vector<float> dxi; // delta[n](x - x_n)
		std::vector<std::vector<float>> dny; // delta^n(y_0)
		std::vector<float> ak;	// a_k
		points_vector_t points; // array of points
		vector2f_t max_x1_x2; // x - min x, y - max x
	};

	class NewtonMethodNonUniformed : public NewtonMethod
	{
	public:
		NewtonMethodNonUniformed() :
			n(0)
		{}

		NewtonMethodNonUniformed(const points_vector_t& points_)
		{
			this->bind(points_);
		}

		void bind(const points_vector_t& points_) override
		{
			this->points = points_;
			if (points.size() < 2) throw std::runtime_error("vector of points is empty");
			n = points.size();
			dxi.resize(n);
			//dny.resize(n);
			ak.resize(n);

			getMinMaxX(points, max_x1_x2);

			for (size_t i = 0; i < n - 1; i++)
				ak[0].emplace_back((points[i + 1].y - points[i].y) / (points[i + 1].x - points[i].x));

			for (size_t k = 1; k < n - 1; k++)
				for (size_t i = 0; i < n - k - 1; i++)
					ak[k].emplace_back((ak[k - 1][i + 1] - ak[k - 1][i]) / (points[i + k + 1].x - points[i].x));
		}

		vector2f_t computeVector(const float x) override
		{
			vector2f_t out_vec = { x, 0 };

			dxi[0] = x - points[0].x;
			for (size_t i = 1; i < n; i++)
				dxi[i] = (x - points[i].x) * dxi[i - 1];

			out_vec.y = points[0].y;
			for (size_t i = 1; i < n; i++)
				out_vec.y = fmadd(ak[i - 1][0], dxi[i - 1], out_vec.y);

			return out_vec;
		}

		points_vector_t computePoints(const float h) override
		{
			if (ak.empty()) throw std::runtime_error("class NewtonMethod is not initialized");
			points_vector_t out_points;
			for (float x = max_x1_x2.y; x <= max_x1_x2.x + h; x += h)
				out_points.push_back(computeVector(x));
			return out_points;
		}

	private:
		size_t n;
		std::vector<float> dxi; // delta[n](x - x_n)
		//std::vector<std::vector<float>> dny; // delta^n(y_0)
		std::vector<std::vector<float>> ak;	// a_k
		points_vector_t points; // array of points
		vector2f_t max_x1_x2; // x - min x, y - max x
	};

	class LagrangeMethod
	{
	public:

		static float compute(const std::vector<float>& px, const std::vector<float>& py,  const float x)
		{
			const size_t n = px.size();
			if (n < 2) throw std::runtime_error("vector of points is empty");
			float polynomial = .0f;
			for (size_t i = 0; i < n; i++)
			{
				float monomial = 1.0f;
				for (size_t j = 0; j < n; j++)
					if (i != j)
						monomial *= (x - px[j]) / (px[i] - px[j]);
				polynomial = fmadd(py[i], monomial, polynomial);
			}
			return polynomial;
		}

		static vector2f_t compute(const points_vector_t& points, const float x)
		{
			if (points.size() < 2) throw std::runtime_error("vector of points is empty");
			const size_t n = points.size();
			float polynomial = .0f;
			for (size_t i = 0; i < n; i++)
			{
				float monomial = 1.0f;
				for (size_t j = 0; j < n; j++)
					if (i != j)
						monomial *= (x - points[j].x) / (points[i].x - points[j].x);
				polynomial = fmadd(points[i].y, monomial, polynomial);
			}
			return { x, polynomial };
		}
	};

	class SplineLinnearMethod
	{
	public:
		static points_vector_t computePoints(const points_vector_t& points, const float h)
		{
			if (points.size() < 2) throw std::runtime_error("vector of points is empty");
			points_vector_t out_points;
			const size_t n = points.size();
			for (size_t i = 0; i < n - 1; i++)
			{
				const float ai = points[i].y;
				const float bi = (points[i + 1].y - points[i].y) / (points[i + 1].x - points[i].x);
				for (float x = points[i].x; x < points[i + 1].x; x += h)
				{
					const float Si = ai + bi * (x - points[i].x);
					out_points.emplace_back(x, Si);
				}
			}
			return out_points;
		}
	};

	class SplineParabolicMethod
	{
	public:
		static points_vector_t computePoints(const points_vector_t& points, const float h)
		{
			if (points.size() < 2) throw std::runtime_error("vector of points is empty");
			points_vector_t out_points;
			const size_t n = points.size();
			std::vector<float> b, c;
			b.resize(n);
			c.resize(n);

			b[0] = 1;
			for (size_t i = 1; i < n; i++)
			{
				const float hx = points[i].x - points[i - 1].x;
				b[i] = (2 * (points[i].y - points[i - 1].y)) / hx - b[i - 1];
			}

			for (size_t i = 0; i < n - 1; i++)
			{
				const float hx = points[i + 1].x - points[i].x;
				c[i] = (b[i + 1] - b[i]) / (2 * hx);
			}

			for (size_t i = 0; i < n - 1; i++)
			{
				for (float x = points[i].x; x <= points[i + 1].x + h; x += h)
				{
					const float Si = points[i].y + b[i] * (x - points[i].x) + c[i] * powf(x - points[i].x, 2);
					out_points.emplace_back(x, Si);
				}
			}
			return out_points;
		}
	};

	class SplineCubicMethod
	{
	public:
		static points_vector_t computePoints(const points_vector_t& points, const float h)
		{
			if (points.size() < 2) throw std::runtime_error("vector of points is empty");
			points_vector_t out_points;
			const size_t n = points.size();
			std::vector<float> b, c, d;
			b.resize(n);
			c.resize(n);
			d.resize(n);

			b[0] = 1;
			for (size_t i = 1; i < n; i++)
			{
				const float hx = points[i].x - points[i - 1].x;
				b[i] = (2 * (points[i].y - points[i - 1].y)) / hx - b[i - 1];
			}

			for (size_t i = 0; i < n - 1; i++)
			{
				const float hx = points[i + 1].x - points[i].x;
				c[i] = (b[i + 1] - b[i]) / (2 * hx);
			}

			for (size_t i = 0; i < n - 1; i++)
			{
				for (float x = points[i].x; x <= points[i + 1].x + h; x += h)
				{
					const float Si = points[i].y + b[i] * (x - points[i].x) + c[i] * powf(x - points[i].x, 2);
					out_points.emplace_back(x, Si);
				}
			}
			return out_points;
		}
	};

}
