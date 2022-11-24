#pragma once
#include <vector>
#include <stdexcept>
#include <iomanip>

namespace slau
{
	using rowf_t = std::vector<float>;
	using matrixf_t = std::vector<rowf_t>;
	void operator -= (rowf_t& row_left, const rowf_t& row_right)
	{
		for (size_t i = 0; i < row_left.size(); i++)
			row_left[i] -= row_right[i];
	}
}

_STD_BEGIN
ostream& operator << (ostream& os, const slau::rowf_t const& row)
{
	for (decltype(auto) value : row)
		os << std::setw(15) << value;
	return os;
}
ostream& operator << (ostream& os, const slau::matrixf_t const& matrix)
{
	for (decltype(auto) row : matrix)
		os << row << std::endl;
	return os;
}
_STD_END

namespace slau
{
	class Gauss
	{
	public:
		static rowf_t computeLinnearSystem(matrixf_t matrix, rowf_t y) 
		{
			if (matrix.size() != y.size())
				throw std::runtime_error("size of matrix not equals to size of y");
			if (y.empty())
				throw std::runtime_error("matrix or y is empty");

			rowf_t x;
			x.resize(y.size());

			//steps 1-3 repeated
			for (size_t i = 0, k = 0; i < y.size(); i++, k++)
			{
				//step 1
				swapMatrixAndY(matrix, y, k, getMax_ik(matrix, k, i));

				//step 2
				normalizeRows(matrix, y, i, k);

				//step 3
				subRowsMatrixAndY(matrix, y, i);
			}
			
			//step 4
			computeX(matrix, y, x);

			return x;
		}

	private:

		// Ищем максимальное значение матрицы по ik
		static size_t getMax_ik(const matrixf_t& matrix, const size_t k, const size_t i_)
		{
			if (i_ == matrix.size() - 1) 
				return matrix.size() - 1;
			size_t i = i_;
			float max_ik = matrix[i][k];
			size_t i_max_ik = i;
			while (++i < matrix.size())
			{
				const float ik = matrix[i][k];
				if (max_ik < ik)
				{
					max_ik = ik;
					i_max_ik = i;
				}
			}
			return i_max_ik;
		}

		// Меняем местами k строку и строку с максимальным значением
		static void swapMatrixAndY(matrixf_t& matrix, rowf_t& y, const size_t k, const size_t i_max_ik)
		{
			if (k == i_max_ik) return;
			std::swap(matrix[k], matrix[i_max_ik]);
			std::swap(y[k], y[i_max_ik]);
		}

		// Нормирование уравнения относительно коэффициента при Xi
		static void normalizeRows(matrixf_t& matrix, rowf_t& y, const size_t i_, const size_t k_)
		{
			for (size_t i = i_; i < matrix.size(); i++)
			{
				const float x = matrix[i][k_];
				matrix[i][k_] = 1;
				for (size_t k = k_ + 1; k < matrix.size(); k++)
					matrix[i][k] /= x;
				y[i] /= x;
			}
		}

		// Вычитание строк
		static void subRowsMatrixAndY(matrixf_t& matrix, rowf_t& y, const size_t i_)
		{
			for (size_t i = i_ + 1; i < matrix.size(); i++)
			{
				matrix[i] -= matrix[i_];
				y[i] -= y[i_];
			}
		}

		// Вычисление X
		static void computeX(const matrixf_t& matrix, rowf_t& y, rowf_t& x)
		{
			for (size_t k = y.size() - 1; k != static_cast<size_t>(-1); k--)
			{
				x[k] = y[k];
				for (int i = 0; i < k; i++)
					y[i] = y[i] - matrix[i][k] * x[k];
			}
		}
	};
}
