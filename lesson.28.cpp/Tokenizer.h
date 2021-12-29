#pragma once

#include <string>
#include <algorithm>
#include <stdexcept>

class Tokenizer
{
protected:
	const std::string& m_str;
	std::string::const_iterator m_itCurr;

public:
	Tokenizer(const std::string& str) : m_str(str) {
		m_itCurr = std::find_if(m_str.begin(), m_str.end(), [](char c) { return std::isspace(c) == 0; });
	}

	bool IsEndOfString() { return m_itCurr == m_str.end(); }

	std::string Next() {
		if (IsEndOfString())
			throw std::invalid_argument(std::string("Next after the end of string") + m_str);
		std::string::const_iterator itCurr = m_itCurr;
		std::string::const_iterator itNext = std::find_if_not(itCurr, m_str.end(), [](char c) { return std::isspace(c) == 0; });
		m_itCurr = std::find_if(itNext, m_str.end(), [](char c) { return std::isspace(c) == 0; });
		return std::string(itCurr, itNext);
	}
};
