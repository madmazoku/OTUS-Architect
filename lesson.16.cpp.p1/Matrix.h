#pragma once

#include <iostream>
#include <fstream>
#include <stdexcept>
#include <vector>
#include <string>
#include <filesystem>
#include <regex>

#include "MatrixFile.h"

class Matrix : public MatrixFile
{
protected:
	friend class Matrix;

	std::vector<double> m_data;
	size_t m_columns;
	size_t m_rows;
	std::string m_file;

	void ReadLine(std::ifstream& inputStream, const std::regex delim = std::regex("\\s+")) {
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

	const Matrix& Read(const std::string& input) {
		m_columns = 0;
		m_rows = 0;
		m_data.clear();
		m_file.clear();

		if (!std::filesystem::exists(input))
			throw std::runtime_error("Input matrix file not exists");
		std::ifstream inputStream(input);
		std::string line;
		if (inputStream.eof())
			throw std::runtime_error("Input matrix file empty");
		while (!inputStream.eof())
			ReadLine(inputStream);
		inputStream.close();
		m_file = input;
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

	Matrix& Write(const std::string& output) {
		if (std::filesystem::exists(output))
			throw std::runtime_error("Output matrix file exists");
		if (m_columns == 0 && m_rows == 0)
			throw std::runtime_error("Can't output empty matrix");
		std::ofstream outputStream(output);
		std::vector<double>::iterator itOutput(m_data.begin());
		size_t columns = 0;
		while (itOutput != m_data.end()) {
			outputStream << std::to_string(*(itOutput++));
			if (++columns == m_columns) {
				outputStream << "\n";
				columns = 0;
			}
			else
				outputStream << "\t";
		}
		outputStream.close();
		m_file = output;
		return *this;
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