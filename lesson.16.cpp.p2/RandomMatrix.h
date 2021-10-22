#pragma once

#include <random>
#include <cmath>

#include "../lesson.16.cpp.p1/Matrix.h"

class RandomMatrix : public Matrix {
public:
	RandomMatrix& Generate(size_t columns, size_t rows) {
		m_columns = columns;
		m_rows = rows;
		m_data.clear();

		std::random_device rd;
		std::default_random_engine re(rd());
		std::uniform_int_distribution<int> ud_x(-10000, 10000);
		while (m_data.size() != m_columns * m_rows)
			m_data.push_back(0.01 * ud_x(re));

		return *this;
	}
};