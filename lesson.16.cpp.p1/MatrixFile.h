#pragma once

#include <string>

class MatrixFile {
protected:
	std::string m_file;

public:
	std::string File() { return m_file; }
};