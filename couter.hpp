#pragma once
#include <string>
#include <vector>
#include <array>
#include <iostream>
#include <iomanip>
#include <ostream>

#define _CN cn::
#define _CN_PRIV cn::_priv::

#define _CN_BEGIN namespace cn {
#define _CN_END } 

#define _CN_PRIV_BEGIN _CN_BEGIN namespace _priv {
#define _CN_PRIV_END } }

#define _CN_ERR_ARGS_NOT_EQ "The number of arguments does not match" \
							"the number of columns in the table"
#define _CN_ERR_DATA_NOT_EQ "Header size is greater than" \
							"the sum of the sizes of all columns"
#ifndef _CN_MYCH

#define _CN_LURD ((char)(206))

#define _CN_CHLU ((char)(201))
#define _CN_CHLD ((char)(200))
#define _CN_CHRU ((char)(187))
#define _CN_CHRD ((char)(188))

#define _CN_CHHOR ((char)(205))
#define _CN_CHVER ((char)(186))

#define _CN_CHUTOD ((char)(203))
#define _CN_CHDTOU ((char)(202))
#define _CN_CHLTOR ((char)(204))
#define _CN_CHRTOL ((char)(185))

#endif //ifndef _CN_MYCH

_CN_BEGIN
using row_idx_t = unsigned __int64;
using spc_sum_t = unsigned __int32;
using spc_idx_t = unsigned __int16;
template <spc_idx_t _Space, spc_idx_t... _Spaces>
class Table;
_CN_END

_CN_PRIV_BEGIN
template <spc_idx_t _Space, spc_idx_t... _Spaces>
struct sum_spaces
{
	static constexpr inline const spc_sum_t sum = _Space + sum_spaces<_Spaces...>::sum;
};

template <spc_idx_t _Space>
struct sum_spaces<_Space>
{
	static constexpr inline const spc_sum_t sum = _Space;
};
_CN_PRIV_END

_CN_PRIV_BEGIN

template<spc_idx_t _Idx_v, typename _Container_t, typename _Left_arg>
constexpr inline void setRow(typename _Container_t& container, typename _Left_arg value)
{
	if constexpr (std::is_same_v<_Left_arg, std::string>)
	{
		container[_Idx_v] = value;
	}
	else if constexpr (std::is_same_v<_Left_arg, const char*> || std::is_same_v<_Left_arg, char*>)
	{
		container[_Idx_v] = std::string(value);
	}
	else
	{
		container[_Idx_v] = std::to_string(value);
	}
}

template<spc_idx_t _Idx_v, typename _Container_t, typename _Left_arg, typename... _Right_args>
constexpr inline void setRow(typename _Container_t& container, typename _Left_arg value, typename _Right_args... values)
{
	if constexpr (std::is_same_v<_Left_arg, std::string>)
	{
		container[_Idx_v] = value;
	}
	else if constexpr (std::is_same_v<_Left_arg, const char*> || std::is_same_v<_Left_arg, char*>)
	{
		container[_Idx_v] = std::string(value);
	}
	else
	{
		container[_Idx_v] = std::to_string(value);
	}
	_CN_PRIV setRow<_Idx_v + 1, _Container_t, _Right_args...>(container, values...);
}
_CN_PRIV_END

_CN_PRIV_BEGIN
enum struct LineType : __int8
{
	HEADER,
	BEGIN,
	INTERIM,
	END
};
_CN_PRIV_END

_CN_BEGIN
	template <spc_idx_t _Space, spc_idx_t... _Spaces>
class Table
{

private:
	static constexpr spc_idx_t _Spaces_cnt = sizeof...(_Spaces) + 1;
	static constexpr spc_idx_t _Spaces_sum = _CN_PRIV sum_spaces<_Space, _Spaces...>::sum;
	static constexpr spc_idx_t _Borders_and_spaces_cnt = _Spaces_sum + _Spaces_cnt + 1;
	static constexpr spc_idx_t _Spaces[_Spaces_cnt] = { _Space, _Spaces... };
	using row_t = std::array<std::string, _Spaces_cnt>;
	std::string m_header;
	row_t m_column_names;
	std::vector<row_t> m_rows;
	row_idx_t m_row_out_lim;

public:
	void SetRowsOutputLimit(row_idx_t row_out_lim)
	{
		m_row_out_lim = row_out_lim;
	}

	void SetHeader(const std::string& header)
	{
		this->m_header = header;
	}

	void SetColumnNames(row_t colums_names)
	{
		this->m_column_names = colums_names;
	}

	void AddRow(row_t row)
	{
		m_rows.push_back(row);
	}

	void SetRow(row_idx_t row_idx, row_t row)
	{
		m_rows[row_idx] = row;
	}

	std::string& GetValue(row_idx_t row_idx, row_idx_t col_idx) const
	{
		return m_rows[row_idx][col_idx];
	}

	void SetValue(row_idx_t row_idx, row_idx_t col_idx, const std::string& value)
	{
		m_rows[row_idx][col_idx] = value;
	}

	template <typename _Value_type>
	void SetValue(row_idx_t row_idx, row_idx_t col_idx, _Value_type value)
	{
		m_rows[row_idx][col_idx] = value;
	}

	row_t& GetRow(row_idx_t row_idx) const
	{
		return m_rows[row_idx];
	}

	row_t& operator [] (row_idx_t row_idx) const
	{
		return m_rows[row_idx];
	}

	template<spc_idx_t _Idx_v = 0, typename _Left_arg, typename... _Right_args>
	void SetColumnNames(typename _Left_arg value, typename _Right_args... values)
	{
		static_assert(
			sizeof...(values) + 1 == _Spaces_cnt,
			_CN_ERR_ARGS_NOT_EQ
		);
		_CN_PRIV setRow<_Idx_v, row_t, _Left_arg, _Right_args...>(this->m_column_names, value, values...);
	}

	void AddRow()
	{
		m_rows.emplace_back();
	}

	void ReserveRows(row_idx_t row_cnt)
	{
		m_rows.reserve(row_cnt);
	}

	void AddRows(row_idx_t row_cnt)
	{
		m_rows.resize(row_cnt);
	}

	template<spc_idx_t _Idx_v = 0, typename _Left_arg, typename... _Right_args>
	void AddRow(typename _Left_arg value, typename _Right_args... values)
	{
		static_assert(
			sizeof...(values) + 1 == _Spaces_cnt,
			_CN_ERR_ARGS_NOT_EQ
		);
		m_rows.emplace_back();
		_CN_PRIV setRow<_Idx_v, row_t, _Left_arg, _Right_args...>(m_rows.back(), value, values...);
	}

	template<spc_idx_t _Idx_v = 0, typename _Left_arg, typename... _Right_args>
	void SetRow(row_idx_t row_idx, typename _Left_arg value, typename _Right_args... values)
	{
		static_assert(
			sizeof...(values) + 1 == _Spaces_cnt,
			_CN_ERR_ARGS_NOT_EQ
			);
		_CN_PRIV setRow<_Idx_v, row_t, _Left_arg, _Right_args...>(m_rows[row_idx], value, values...);
	}


private:
	void m_writeLine(
		std::string& line_str,
		const char ch0, const char ch1, 
		const char ch2, const char ch3
	) const
	{
		line_str.push_back(ch0);
		for (spc_idx_t space_idx = 0; space_idx < _Spaces_cnt; space_idx++)
		{
			for (spc_idx_t i = 0; i < _Spaces[space_idx]; i++)
			{
				line_str.push_back(ch1);
			}
			if (space_idx != _Spaces_cnt - 1)
			{
				line_str.push_back(ch2);
			}
		}
		line_str.push_back(ch3);
	}

	void m_coutLine(std::ostream& os, const _CN_PRIV LineType line_type) const
	{
		std::string line_str;

		switch (line_type)
		{
		case _CN_PRIV LineType::HEADER:
			m_writeLine(line_str, _CN_CHLU, _CN_CHHOR, _CN_CHHOR, _CN_CHRU);
			break;
		case _CN_PRIV LineType::BEGIN:
			m_writeLine(line_str, _CN_CHLTOR, _CN_CHHOR, _CN_CHUTOD, _CN_CHRTOL);
			break;
		case _CN_PRIV LineType::INTERIM:
			m_writeLine(line_str, _CN_CHLTOR, _CN_CHHOR, _CN_LURD, _CN_CHRTOL);
			break;
		case _CN_PRIV LineType::END:
			m_writeLine(line_str, _CN_CHLD, _CN_CHHOR, _CN_CHDTOU, _CN_CHRD);
			break;
		default:
			break;
		}

		os << line_str << std::endl;
	}

	void m_coutRow(std::ostream& os, const row_t& row, bool is_data_row = false, row_idx_t row_idx = 0ULL) const
	{
		os << _CN_CHVER;
		for (spc_idx_t space_idx = 0; space_idx < _Spaces_cnt; space_idx++)
		{
			const spc_idx_t row_data_size = row[space_idx].size();
			const spc_idx_t setw_empty_v = _Spaces[space_idx] - row_data_size;
			const spc_idx_t setw_v = setw_empty_v / 2;
			const spc_idx_t setw_left_v = row_data_size + setw_v;
			spc_idx_t setw_right_v = 0 + setw_v + (row_data_size % 2) + (_Spaces[space_idx] % 2 == 0);
			if (row_data_size > _Spaces[space_idx])
			{
				throw std::runtime_error(_CN_ERR_DATA_NOT_EQ);
			}
			if (row_data_size % 2 == 0 && _Spaces[space_idx] % 2 == 1)
			{
				setw_right_v = setw_right_v + 2;
			}
			os
				<< std::setw(setw_left_v)
				<< row[space_idx]
				<< std::setw(setw_right_v)
				<< _CN_CHVER;
		}
		os
			<< std::endl;
		if (is_data_row)
		{
			row_idx_t row_out_lim = m_row_out_lim == 0 ? m_rows.size() : m_row_out_lim;
			if (row_idx != row_out_lim - 1)
			{
				m_coutLine(os, _CN_PRIV LineType::INTERIM);
			}
			else
			{
				m_coutLine(os, _CN_PRIV LineType::END);
			}
		}
	}

public:
	std::ostream& CoutTable(std::ostream& os) const
	{
		m_coutLine(os, _CN_PRIV LineType::HEADER);
		if (this->m_header.size() == _Borders_and_spaces_cnt)
		{
			os << m_header << std::endl;
		}
		else if (this->m_header.size() < _Borders_and_spaces_cnt)
		{
			static const spc_idx_t header_size = m_header.size();
			const spc_idx_t setw_empty_v = _Borders_and_spaces_cnt - header_size;
			const spc_idx_t setw_v = setw_empty_v / 2;
			const spc_idx_t setw_left_v = header_size + setw_v;
			const spc_idx_t setw_right_v = 0 + setw_v - (header_size % 2) + (_Spaces_sum % 2) - (_Spaces_cnt % 2);
			os 
				<< _CN_CHVER 
				<< std::setw(setw_left_v) 
				<< m_header 
				<< std::setw(setw_right_v)
				<< _CN_CHVER 
				<< std::endl;
		}
		else
		{
			throw std::runtime_error(_CN_ERR_DATA_NOT_EQ);
		}
		m_coutLine(os, _CN_PRIV LineType::BEGIN);
		m_coutRow(os, m_column_names);
		m_coutLine(os, _CN_PRIV LineType::INTERIM);
		row_idx_t row_out_lim = m_row_out_lim == 0 ? m_rows.size() : m_row_out_lim;
		for (row_idx_t row_idx = 0ULL; row_idx < row_out_lim; row_idx++)
		{
			m_coutRow(os, m_rows[row_idx], true, row_idx);
		}
		return os;
	}
};
_CN_END

_STD_BEGIN
template<_CN spc_idx_t _Space, _CN spc_idx_t ..._Spaces>
ostream& operator << (ostream& os, const cn::Table<_Space, _Spaces...> const& table)
{
	return table.CoutTable(os);
}
_STD_END

