#pragma once

#include <iostream>
#include <fstream>
#include <stdexcept>
#include <vector>
#include <string>
#include <filesystem>
#include <regex>

class Matrix
{
protected:
	std::vector<double> m_data;
	size_t m_columns;
	size_t m_rows;

	void ReadLine(std::istream& inputStream, const std::regex delim = std::regex("\\s+")) {
		std::string line;
		std::getline(inputStream, line);
		if (line.empty() || std::regex_match(line, delim))
			return;
		std::sregex_token_iterator pos(line.begin(), line.end(), delim, -1), end;
		size_t columns = 0;
		while (pos != end) {
			m_data.push_back(std::stod(*pos));
			++pos;
			++columns;
		}

		if (m_columns == 0)
			m_columns = columns;
		else if (m_columns != columns)
			throw std::runtime_error(std::string("Columns count mismatch at row ") + std::to_string(m_rows) + " in input matrix");
		++m_rows;
	}
public:
	Matrix() : m_columns(0), m_rows(0) {}
	Matrix(std::vector<double> data, size_t columns, size_t rows) : m_data(std::move(data)), m_columns(columns), m_rows(rows) {}

	const Matrix& Read(const std::string& input) {
		if (!std::filesystem::exists(input))
			throw std::runtime_error("Input matrix file not exists");
		std::ifstream inputStream(input);
		Read(inputStream);
		inputStream.close();
		return *this;
	}

	const Matrix& Read(std::istream& inputStream) {
		m_columns = 0;
		m_rows = 0;
		m_data.clear();

		std::string line;
		if (inputStream.eof())
			throw std::runtime_error("Input matrix file empty");
		while (!inputStream.eof())
			ReadLine(inputStream);
		return *this;
	}

	const Matrix& operator+=(const Matrix& m) {
		if (m_columns == 0 && m_rows == 0) {
			m_columns = m.m_columns;
			m_rows = m.m_rows;
			m_data = m.m_data;
		}
		else {
			if (m_columns != m.m_columns || m_rows != m.m_rows)
				throw std::runtime_error("Input matrix dimensions micmatch");
			std::vector<double>::iterator itOutput(m_data.begin());
			std::vector<double>::const_iterator itInput(m.m_data.begin());
			while (itOutput != m_data.end())
				*(itOutput++) += *(itInput++);
		}
		return *this;
	}

	void Write(const std::string& output) const {
		if (std::filesystem::exists(output))
			throw std::runtime_error("Output matrix file exists");
		std::ofstream outputStream(output);
		Write(outputStream);
		outputStream.close();
	}

	void Write(std::ostream& outputStream) const {
		if (m_columns == 0 && m_rows == 0)
			throw std::runtime_error("Can't output empty matrix");
		size_t columns = 0;
		for (double d : m_data) {
			outputStream << d;
			if (++columns == m_columns) {
				outputStream << "\n";
				columns = 0;
			}
			else
				outputStream << "\t";
		}
	}

	bool Compare(const Matrix& m, double eps) {
		if (m_columns != m.m_columns || m_rows != m.m_rows)
			throw std::runtime_error("Compared matrix dimensions micmatch");
		std::vector<double>::iterator itOutput(m_data.begin());
		std::vector<double>::const_iterator itInput(m.m_data.begin());
		while (itOutput != m_data.end())
			if (std::fabs(*(itOutput++) - *(itInput++)) > eps)
				return false;
		return true;
	}
};